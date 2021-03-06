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
#include <algorithm>

constexpr auto SERVERDATA_AUTH = 3;
constexpr auto SERVERDATA_EXECCOMMAND = 2;
constexpr auto SERVERDATA_AUTH_RESPONSE = 2;
constexpr auto SERVERDATA_RESPONSE_VALUE = 0;

static const QByteArray PacketEmpty = []() {
    static qint8 data[] = { 0x0, 0x1, 0x0, 0x0, 0x0, 0x0 };
    return QByteArray(reinterpret_cast<const char*>(data), sizeof(data));
}();


QRconSession::QRconSession(QObject* parent) :
    QObject(parent),
    m_socket(new QTcpSocket(this))
{   
    connect(m_socket, &QAbstractSocket::readyRead, this, &QRconSession::readRcon);
    connect(m_socket, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), this, &QRconSession::handleError);
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

void QRconSession::command(QRconCommand* command)
{
    auto it = std::find_if(m_commands.begin(), m_commands.end(), [command](auto it) {
        return it == command;
    });

    if (it == m_commands.end())
        m_commands << QPointer<QRconCommand>(command);

    quint32 id;
    QByteArray packet = makePacket(SERVERDATA_EXECCOMMAND, command->command(), &id);
    command->commandId = id;
    command->clear();
    m_socket->write(packet);

    packet = makePacket(SERVERDATA_RESPONSE_VALUE, QString(), &id);
    command->verifyId = id;
    m_socket->write(packet);
}

void QRconSession::setServerConfig(const QRconServerConfig &config)
{
    setHostName(config.hostName());
    setPassword(config.password());
    setPort(config.port());
}

void QRconSession::setHostName(const QString& hostName)
{
    m_hostName = hostName;
}

void QRconSession::setPassword(const QString& password)
{
    m_password = password;
}

void QRconSession::setPort(quint16 port)
{
    m_port = port;
}

void QRconSession::rconPacketReceived(qint32 id, qint32 type, const QByteArray &body)
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
        // remove all destroyed commands
        m_commands.erase(std::remove_if(m_commands.begin(), m_commands.end(), [](auto it) { return it.isNull(); }), m_commands.end());

        auto it = std::find_if(m_commands.begin(), m_commands.end(), [id](auto it) {
            return it->commandId == static_cast<quint32>(id) || it->verifyId == static_cast<quint32>(id);
        });
        
        if (it != m_commands.end()) {
            if (static_cast<quint32>(id) == (*it)->verifyId && body == PacketEmpty) { // empty response
                (*it)->finish();
            } else {
                (*it)->replyReceived(QString(body));
            }
        }
    }
}

QByteArray QRconSession::makePacket(qint32 packetType, const QString& string, quint32 *id)
{
    qint32 length = string.length() + 10;
    qint32 type = packetType;

    m_id += 1;
    if (id)
        *id = m_id;
    
    QByteArray packet;
    packet.append(reinterpret_cast<char*>(&length), 4);
    packet.append(reinterpret_cast<char*>(&m_id), 4);
    packet.append(reinterpret_cast<char*>(&type), 4);
    packet.append(string);
    packet.append('\0').append('\0');
    
    return packet;
}

void QRconSession::authenticateImpl()
{
    qDebug("Using password: %s", qPrintable(password()));

    QByteArray packet = makePacket(SERVERDATA_AUTH, password(), &m_authId);
    m_socket->write(packet);
}

void QRconSession::readRcon()
{
    QDataStream stream(m_socket);
    stream.setByteOrder(QDataStream::LittleEndian);
    
    while (m_socket->bytesAvailable() > 0) {
        if (m_length == 0) {
            if (m_socket->bytesAvailable() < 4)
                return;
            
            stream >> m_length;
        }
        
        if (m_socket->bytesAvailable() < m_length)
            return;
        
        qint32 id, type;
        stream >> id >> type;
        
        QByteArray body(m_length - 8, '\0');
        stream.readRawData(body.data(), m_length - 8);
        
        rconPacketReceived(id, type, body);
        m_length = 0;
    }
}

void QRconSession::handleError(QAbstractSocket::SocketError socketError)
{
    /* TODO Handle errors properly */
    if (socketError == QAbstractSocket::RemoteHostClosedError)
        emit error(Disconnected);
    else
        emit error(Other);
}

