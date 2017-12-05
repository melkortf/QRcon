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

#include "qrconserverconfig.h"
#include "qrcon_export.h"
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpSocket>

class QRconCommand;

/**
 * \ingroup QRcon
 * @{
 * 
 * The \c QRconSession class represents a single RCON session. It starts
 * with authentication and ends when the connection is closed.
 */
class QRCON_EXPORT QRconSession : public QObject {
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
    Q_PROPERTY(quint16 port READ port WRITE setPort)
    
public:
    /**
     * The \c Error enum describes an error that happened during working with
     * the RCON session.
     */
    enum Error {
        AuthenticationFailed    /**< Indicates the wrong RCON authentication */,
        Disconnected            /**< The connection with RCON host is lost */,
        Other                   /**< Other, internal error */
    };
    Q_ENUM(Error)
    
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
     * Executes the command and stores the reply in it.
     */
    void command(QRconCommand* command);

    /**
     * Sets the \c hostName, \c password and \c port at once, using the
     * \c config provided.
     */
    void setServerConfig(const QRconServerConfig& config);
    
    const QString& hostName() const { return m_hostName; }
    void setHostName(const QString& hostName);
    const QString& password() const { return m_password; }
    void setPassword(const QString& password);
    quint16 port() const { return m_port; }
    void setPort(quint16 port);
    
protected:
    virtual void rconPacketReceived(qint32 id, qint32 type, const QByteArray& body);
    
private:
    QByteArray makePacket(qint32 packetType, const QString& string, quint32* id = nullptr);
    
private slots:
    void authenticateImpl();
    void readRcon();
    void handleError(QAbstractSocket::SocketError socketError);
    
private:
    QString m_hostName;
    QString m_password;
    quint16 m_port = 27015; /* Default port */
    QTcpSocket* m_socket = nullptr;
    quint32 m_id = 0; /* Packet counter */
    quint32 m_authId = 0; /* ID of the auth packet */
    int m_authenticated = 0; /* 0, 1 - not authenticated, 2 - authenticated */
    QList<QPointer<QRconCommand>> m_commands; /* List of pending commands */
    int m_length = 0; /* Used to read RCON packets */
    QString m_body;
    
}; /** @} */

#endif // QRCONSESSION_H
