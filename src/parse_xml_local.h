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

#ifndef PARSE_XML_LOCAL_H
#define PARSE_XML_LOCAL_H

#include <QFile>
#include <QDebug>
#include <QIODevice>
#include <QTableWidget>
#include <QDomDocument>
#include <QMessageBox>

#include "definitionen.h"

class Parse_XML_Local: public QObject {

public:
    Parse_XML_Local(QObject *parent = nullptr);

    bool readFile(QTableWidget *tableWidget, QIODevice *xmlDevice);

    QString errorString() const;

private:
//    QStringList saveGameInfo(QDomElement domElement);
//    bool gameName(QDomElement domElement);
//    bool gameDate(QDomElement domElement);
//    bool gameSize(QDomElement domElement);

    QDomDocument domDocument;

    QIcon simutransIcon;

};

#endif // PARSE_XML_LOCAL_H
