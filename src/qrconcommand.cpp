/*
 * qrconcommand.cpp
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

#include "qrconcommand.h"
#include <QtCore>

QRconCommand::QRconCommand(const QString& command, QObject* parent) :
    QObject(parent),
    m_command(command) {}

void QRconCommand::clear()
{
    m_body.clear();
}

void QRconCommand::replyReceived(const QString& body)
{
    m_body.append(body);
}

void QRconCommand::finish()
{
    emit finished();
}
