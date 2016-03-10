/*
 * qrconcommand.h
 * Copyright (C) 2016 Michał Garapich <michal@garapich.pl>
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

#ifndef QRCONCOMMAND_H
#define QRCONCOMMAND_H

#include "qrconexport.h"
#include <QObject>

class QRconSession;

/**
 * \ingroup QRcon
 * @{
 * 
 * The \c QRconCommand class represents a single RCON command that can be
 * executed during the session.
 * 
 * Although we can execute commands in a simple way, using
 * \ref QRconSession::command(), there is no simple way to capture the server's
 * response. This class makes it easy to read the reply.
 */
class __QRconApi__ QRconCommand : public QObject {
    Q_OBJECT
    friend class QRconSession;
    
signals:
    /**
     * Emitted when the command finishes its execution.
     * The response can be read using the \ref body() method.
     */
    void finished();

public:
    explicit QRconCommand(const QString& command, QObject* parent = nullptr);
    
    const QString& command() const { return m_command; }
    const QByteArray& body() const { return m_body; }
    
protected:
    virtual void replyReceived(const QByteArray& body);
    
private:
    int commandId;
    QString m_command;
    QByteArray m_body;

}; /** @} */

#endif // QRCONCOMMAND_H
