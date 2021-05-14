/******************************************************************************
**
** Copyright(C) by Georg Lassnig
**
** Contact e-mail     : g.lassnig@e67-its.de
** Program URL        : http://www.e67-its.de/
**
** Developed with     : Qt Creator
** Programing Language: C++
** Depend on          : Qt4/Qt5
**
** License Information:
**
** $QT_BEGIN_LICENSE:GPL$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; WITHOUT even the implied WARRANTY OF
** MERCHANTABILITY or FITNESS for A PARTICULAR PURPOSE. See the
** GNU General PUBLIC License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/> .
**
** $QT_END_LICENSE$
**
*******************************************************************************/

#ifndef DEFINITIONEN_H
#define DEFINITIONEN_H

#include <QDir>
#include <QString>

#define APP_VERSION "3.0-alpha"
#define APP_TITLE "Simutrans Starter"
#define APP_NAME "simurun"
#define APP_CONFIG_PATH QDir::homePath()+"/."+ APP_NAME + "/"
#define MAX_MONEY 1000000000
#define APPS_TPL_PATH "/usr/share/doc/packages/" + QString(APP_NAME) + "/tpl/"
#define LANG_PATH "/usr/share/doc/packages/" + QString(APP_NAME) + "/locale/"
#define FLAGS_PATH "/usr/share/doc/packages/" + QString(APP_NAME) + "/flags/"
#define HELP_PATH "/usr/share/doc/packages/" + QString(APP_NAME) + "/doc/"
#define DESKTOP_FILE "/usr/share/applications/"
#define CACHED_XML "/save/_cached.xml"

#endif // DEFINITIONEN_H
