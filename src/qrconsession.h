/*
 * qrconsession.h
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

#ifndef QRCONSESSION_H
#define QRCONSESSION_H

#include "qrconexport.h"
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>

class QRconCommand;

/**
 * \ingroup QRcon
 * @{
 * 
 * The \c QRconSession class represents a single RCON session. It starts
 * with authentication and ends when the connection is closed.
 */
class __QRconApi__ QRconSession : public QObject {
    Q_OBJECT
    
    /**
     * The \c hostName property keeps the address of the remote host that
     * has the RCON.
     */
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName)
    
    /**
     * The \c password property contains the password using to authenticate
     * to the RCON.
     */
    Q_PROPERTY(QString password READ password WRITE setPassword)
    
    /**
     * The \c port property specifies the port on which the RCON server is
     * listening. By default it is 27015.
     */
    Q_PROPERTY(quint32 port READ port WRITE setPort)
    
public:
    /**
     * The \c Error enum describes an error that happened during working with
     * the RCON session.
     */
    enum Error {
        AuthenticationFailed /**< Indicates the wrong RCON authentication */,
        Disconnected /**< The connection with RCON host is lost */
    };
    
signals:
    /**
     * Emitted when user is authenticated successfully.
     */
    void authenticated();
    
    /**
     * Emitted when an error occurs.
     */
    void error(Error error);

public:
    /**
     * Creates a new QRconSession. Passes \c parent to \c QObject.
     */
    explicit QRconSession(QObject* parent = nullptr);
    
    /**
     * Closes and destroys the QRconSession.
     */
    virtual ~QRconSession();
    
    /**
     * Tries to authenticate to RCON using the \ref hostName, \ref port
     * and \ref password properties. Successfull authentication is signalized
     * with the \ref authenticated() signal.
     */
    void authenticate();
    
    /**
     * Issues a single command over the RCON protocol.
     */
    void command(const QString& command);
    
    /**
     * Executes the command and stores the reply in it.
     */
    void command(QRconCommand* command);
    
    const QString& hostName() const { return m_hostName; }
    void setHostName(const QString& hostName);
    const QString& password() const { return m_password; }
    void setPassword(const QString& password);
    quint32 port() const { return m_port; }
    void setPort(quint32 port);
    
protected:
    virtual void rconPacketReceived(qint32 id, qint32 type, const QByteArray& body);
    
private:
    QByteArray makePacket(qint32 packetType, const QString& string);
    
private slots:
    void authenticateImpl();
    void readRcon();
    void handleError(QAbstractSocket::SocketError error);
    
private:
    QString m_hostName;
    QString m_password;
    quint32 m_port = 27015; /* Default port */
    QTcpSocket* m_socket = nullptr;
    quint32 m_id = 0; /* Packet counter */
    quint32 m_authId = 0; /* ID of the auth packet */
    int m_authenticated = 0; /* 0, 1 - not authenticated, 2 - authenticated */
    QList<QRconCommand*> m_commands; /* List of pending commands */
    
}; /** @} */

#endif // QRCONSESSION_H
