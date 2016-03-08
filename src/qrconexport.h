/*
 * qrconexport.h
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

#ifndef QRCON_EXPORT_H
#define QRCON_EXPORT_H

#include <QtGlobal>

#ifndef WITH_STATIC
# ifdef qrcon_EXPORTS
#  define __QRconApi__ Q_DECL_EXPORT
# else
#  define __QRconApi__ Q_DECL_IMPORT
# endif
#else
# define __QRconApi__
#endif

#define __QRconHide__

#endif // QRCON_EXPORT_H
