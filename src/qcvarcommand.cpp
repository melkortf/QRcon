/*
* qcvarcommand.cpp
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

#include "qcvarcommand.h"
#include <QtCore>

QCvarCommand::QCvarCommand(const QString& cvarname, QObject* parent) :
    QRconCommand(cvarname, parent)
{
    connect(this, &QRconCommand::finished, this, &QCvarCommand::parse);
}

void QCvarCommand::parse()
{
    QRegularExpression re("^\\\"" + command() + "\\\"\\s=\\s\\\"(.[^\\\"]+)\\\".*$");

    auto lines = body().split('\n');
    for (auto l : lines) {
        auto match = re.match(l);
        if (match.hasMatch()) {
            m_value = match.captured(1);
            break;
        }
    }
}
