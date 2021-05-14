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

#include "parse_xml_local.h"

Parse_XML_Local::Parse_XML_Local(QObject *parent) : QObject(parent) {

    simutransIcon.addPixmap(QPixmap(":/images/images/simutrans.png"));
}

bool Parse_XML_Local::readFile(QTableWidget *tableWidget, QIODevice *xmlDevice) {
    QString errorStr;
    int errorLine;
    int errorColumn;
    QMessageBox msgBox;

    if (!domDocument.setContent(xmlDevice, true, &errorStr, &errorLine,
                                &errorColumn)) {
        msgBox.setWindowTitle(tr("%1 - Fehlermeldung").arg(APP_TITLE));
        msgBox.setWindowIcon(QIcon(":/icons/icons/info.png"));
        msgBox.setText(tr("<b>Beim einlesen der Datei ist ein Fehler aufgetreten!</b>"));
        msgBox.setInformativeText(tr("Parse error at line %1, column %2:\n%3")
                                  .arg(errorLine)
                                  .arg(errorColumn)
                                  .arg(errorStr));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return false;
    }

    QDomElement root = domDocument.documentElement();
//    saveGameInfo(root.firstChildElement("Simutrans"));

    QDomElement childName = root.firstChildElement("Simutrans");
    childName.firstChildElement("Simutrans");
    while (!childName.isNull()) {

        QStringList strList;
        QString strValue;
        QDomElement domElement = root.firstChildElement("save_game_info");

        domElement = domElement.nextSiblingElement("save_game_info");
        strValue = domElement.attribute("CDATA");
//        strValue += " || ";
//        strValue += domElement.attribute("CDATA");
//        strValue += " || ";
//        strValue += domElement.firstChildElement("i64").text();
//        strValue += " || ";
//        strValue += domElement.firstChildElement("i32").text();
        strList << strValue;
        domElement = domElement.nextSiblingElement("save_game_info");

        qDebug() << "qq";
    }

    // Kopfzeile für die Tabellenansicht.
    // Header for the table widget.
    QStringList header;
    header << tr("Spiel")
           << tr("Pak-Set")
           << tr("Gespeichert");


    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(header);

    return true;
}

//QStringList Parse_XML_Local::saveGameInfo(QDomElement domElement){

//    QStringList strList;
//    QString strValue;

//    while (!domElement.isNull()) {
//        strValue = domElement.attribute("CDATA");
//        strValue += " || ";
//        strValue += domElement.attribute("CDATA");
//        strValue += " || ";
//        strValue += domElement.firstChildElement("i64").text();
//        strValue += " || ";
//        strValue += domElement.firstChildElement("i32").text();
//        strList << strValue;
//        domElement = domElement.nextSiblingElement("save_game_info");
//    }

//    qDebug() << strList;

//    return strList;
//}

//bool Parse_XML_Local::gameName(QDomElement domElement){

//    QStringList strList;
//    QString strValue;

//    while (!domElement.isNull()) {

//    }
//}
