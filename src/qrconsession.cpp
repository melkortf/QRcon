/*
 * qrconsession.cpp
 * Copyright (C) 2016  Michał Garapich <michal@garapich.pl>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "qrconsession.h"
#include "qrconcommand.h"
#include <QtNetwork>
#include <cstring>
#include <functional>

constexpr auto SERVERDATA_AUTH = 3;
constexpr auto SERVERDATA_EXECCOMMAND = 2;
constexpr auto SERVERDATA_AUTH_RESPONSE = 2;
constexpr auto SERVERDATA_RESPONSE_VALUE = 0;


QRconSession::QRconSession(QObject* parent) :
    QObject(parent),
    m_socket(new QTcpSocket(this))
{   
    connect(m_socket, &QAbstractSocket::readyRead, this, &QRconSession::readRcon);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    connect(m_socket, &QAbstractSocket::connected, this, &QRconSession::authenticateImpl);
}

QRconSession::~QRconSession()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();
}

void QRconSession::authenticate()
{
    Q_ASSERT(m_socket);
    
    qDebug("Connecting to %s on port %d", qPrintable(hostName()), port());
    m_socket->connectToHost(hostName(), port());
}

void QRconSession::command(const QString& command)
{
    QByteArray packet = makePacket(SERVERDATA_EXECCOMMAND, command);
    m_socket->write(packet);
}

void QRconSession::command(QRconCommand* command)
{
    m_commands << command;
    command->commandId = m_id + 1;
    this->command(command->command()); // wtf is that construction
}

void QRconSession::setHostName(const QString& hostName)
{
    m_hostName = hostName;
}

void QRconSession::setPassword(const QString& password)
{
    m_password = password;
}

void QRconSession::setPort(quint32 port)
{
    m_port = port;
}

void QRconSession::rconPacketReceived(qint32 id, qint32 type, const QByteArray& body)
{
    if (id == static_cast<qint32>(m_authId)) {
        if (type == SERVERDATA_RESPONSE_VALUE || type == SERVERDATA_AUTH_RESPONSE) {
            m_authenticated += 1;
        }
        
        if (m_authenticated == 2) {
            emit authenticated();
        }
    } else if (id == -1) {
        qDebug("Authentication failed!");
        emit error(AuthenticationFailed);
    } else if (type == SERVERDATA_RESPONSE_VALUE) {
        auto it = std::find_if(m_commands.begin(), m_commands.end(), [id](auto it) {
            return it->commandId == id;
        });
        
        if (it != m_commands.end()) {
            (*it)->replyReceived(body);
        }
    }
    
    Q_UNUSED(body);
}

QByteArray QRconSession::makePacket(qint32 packetType, const QString& string)
{
    qint32 length = string.length() + 10;
    qint32 id = ++m_id;
    qint32 type = packetType;
    
    QByteArray packet;
    packet.append(reinterpret_cast<char*>(&length), 4);
    packet.append(reinterpret_cast<char*>(&id), 4);
    packet.append(reinterpret_cast<char*>(&type), 4);
    packet.append(string);
    packet.append('\0').append('\0');
    
    return packet;
}

void QRconSession::authenticateImpl()
{
    qDebug("Using password: %s", qPrintable(password()));
    
    connect(m_socket, &QAbstractSocket::disconnected, std::bind(&QRconSession::error, this, Disconnected));
    
    QByteArray packet = makePacket(SERVERDATA_AUTH, password());
    m_authId = m_id;
    m_socket->write(packet);
}

void QRconSession::readRcon()
{
    qDebug("%lld bytes available", m_socket->bytesAvailable());
    
    do {
        qint32 length, id, type;
        qint64 bytes = m_socket->read(reinterpret_cast<char*>(&length), 4);
        Q_ASSERT(bytes == 4);
        bytes = m_socket->read(reinterpret_cast<char*>(&id), 4);
        Q_ASSERT(bytes == 4);
        bytes = m_socket->read(reinterpret_cast<char*>(&type), 4);
        Q_ASSERT(bytes == 4);
        
        QByteArray body = m_socket->read(length - 8);
        rconPacketReceived(id, type, body);
        
    } while (m_socket->bytesAvailable() >= 14); // 14 is minimum packet size
}

void QRconSession::handleError(QAbstractSocket::SocketError error)
{
    /* TODO Handle me please */
    Q_UNUSED(error);
}

