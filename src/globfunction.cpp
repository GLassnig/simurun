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

#include "globfunction.h"

/*
** Globale MessageBox für nicht implementierte Funktionen im Programm.
** Global MessageBox for not implemented features.
**/
void GlobFunction::global_message(QString meldung) {

    QMessageBox msgBox;
    msgBox.setWindowTitle(APP_TITLE);
    msgBox.setIcon(msgBox.Information);
    msgBox.setText(meldung);
    msgBox.exec();
  }

void GlobFunction::read_simutrans_version() {
    QProcess *myProcess = new QProcess();
    QString read_PakSets = APP_CONFIG_PATH + "read_version.sh";
    myProcess->execute(read_PakSets);
  }

void GlobFunction::read_simutrans_version_script(QString gamePath) {
    // Ein Script das bei jedem Programmstart die
    // Simutrans Version ausliest.
    QFile file(APP_CONFIG_PATH + "read_version.sh");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "#!/bin/sh\n"
        << "# This file is part of Simutrans Starter!\n"
        << "# Do not edit or remove the file.\n"
        << "cat " + gamePath +
           "/history.txt | grep 'Release of ' | sed s/'Release of '//g > " +
           APP_CONFIG_PATH + "/SimutransVersion.lst";
    QProcess *myProcess = new QProcess();
    myProcess->execute("chmod 755 " + APP_CONFIG_PATH + "read_version.sh");
  }

void GlobFunction::install_Archive(QString instPath, QString instArchive) {
    QProcess *decompressArchive = new QProcess;
    QProcess *copyArchive = new QProcess;
    QProcess *removeTempDir = new QProcess;
	QFileInfo fileSuffix(instArchive);
	QString extension = fileSuffix.suffix();

	if(extension == "7z") {
		// Das zu installierende 7-Zip Archive entpacken.
		decompressArchive->execute("7z x -y " + instArchive + " -o" +
								   QDir::tempPath() + "/simuinstall");
	} else {
		// Das zu installierende Archive entpacken.
		decompressArchive->execute("unzip -o " + instArchive + " -d " +
								   QDir::tempPath() + "/simuinstall");

	}

    // Die entpackten Dateien in das Simutrans-Verzeichnis kopieren.
	instPath = instPath.replace(QString("/simutrans"), QString("/"));
    if (decompressArchive->waitForFinished() == false) {
        copyArchive->execute("cp -vfr " + QDir::tempPath() +
                             "/simuinstall/simutrans/" + " " + instPath);
      }

    if (copyArchive->waitForFinished() == false) {
        removeTempDir->execute("rm -vfr " + QDir::tempPath() + "/simuinstall");
      }
  }


