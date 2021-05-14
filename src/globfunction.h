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

#ifndef GLOBFUNCTION_H
#define GLOBFUNCTION_H

#include <QTextStream>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QDirModel>

#include "definitionen.h"

class GlobFunction {

  public:
    //GlobFunction();

    void global_message(QString meldung);

    void read_paksets();
    void read_paksets_script(QString gamePath);

    void read_savedgames();
    void read_savedgames_script();

    void read_simutrans_version();
    void read_simutrans_version_script(QString gamePath);

    void install_Archive(QString instPath, QString instArchive);
  };

#endif // GLOBFUNCTION_H
