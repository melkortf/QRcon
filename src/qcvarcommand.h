/*
* qcvarcommand.h
* Copyright (C) 2016 Micha³ Garapich <michal@garapich.pl>
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

#ifndef QCVARCOMMAND_H
#define QCVARCOMMAND_H

#include "qrconexport.h"
#include "qrconcommand.h"

/**
* \ingroup QRcon
* @{
*
* The QCvarCommand can be used to retrieve cvars from the remote server.
*/
class __QRconApi__ QCvarCommand : public QRconCommand {
    Q_OBJECT
    
public:
    explicit QCvarCommand(const QString& cvarname, QObject* parent = nullptr);

    const QString& value() const { return m_value; }

private slots:
    void parse();

private:
    QString m_value;

}; /** @} */

#endif // QCVARCOMMAND_H
