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

#include "qrcon_export.h"
#include <QtCore/QObject>

class QRconSession;

/**
 * \ingroup QRcon
 * @{
 * 
 * The \c QRconCommand class represents a single RCON command that can be
 * executed during the session.
 */
class QRCON_EXPORT QRconCommand : public QObject {
    Q_OBJECT
    
signals:
    /**
     * Emitted when the command finishes its execution.
     * The response can be read using the \ref body() method.
     */
    void finished();

public:
    explicit QRconCommand(const QString& command, QObject* parent = nullptr);
    
    /**
     * The command that is executed.
     */
    const QString& command() const { return m_command; }

    /**
     * A response body.
     * This method returns a null QString unless \ref finished() was already emitted.
     */
    const QString& body() const { return m_body; }
    
protected:
    /**
     * Clears the response body, preparing it for a next execution.
     */
    void clear();
    
private:
    friend class QRconSession;

    void replyReceived(const QString& body);
    void finish();

    quint32 commandId, verifyId;
    QString m_command;
    QString m_body;

}; /** @} */

#endif // QRCONCOMMAND_H
