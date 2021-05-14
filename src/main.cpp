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

/* KDE Headers */

/* Qt Headers */
#include <QtGui>
#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QDir>
#include <QLibraryInfo>

#include "simurun.h"
#include "setupgame.h"
#include "definitionen.h"
#include "globfunction.h"
#ifdef Q_OS_LINUX
#include "QtSingleApplication"
#else //Q_OS_WIN
#include "qtsingleapplication.h"
#endif

int main(int argc, char *argv[]) {
    QDir confDir;
    QFile confFile;

    QtSingleApplication app(argc, argv);
    QTranslator myappTranslator;

    QString locale = QLocale::system().name();
    QString defaultLanguage = QString(APP_NAME) + "_" + locale + ".qm";
    QString currLanguage, gamePath;
    bool languageSet;

    // Falls die Sprache manuell gesetzt wurde.
    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    languageSet = simurun_conf.value("Set_Language", false).toBool();
    currLanguage = simurun_conf.value(
                     "languageFileName", defaultLanguage).toString();
    gamePath = simurun_conf.value("GamePath").toString();

    simurun_conf.endGroup();

    /*
     * Get the version of Simutrans.
     */
    GlobFunction read_version_script;
    read_version_script.read_simutrans_version_script(gamePath);

    // Systemsprache aus der Umgebungsvariable des Systems lesen.
    // Read the system language from the system environment.
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    if (languageSet == false) {
        myappTranslator.load(LANG_PATH + APP_NAME + "_"  + locale + ".qm");
        app.installTranslator(&myappTranslator);
      } else {
        myappTranslator.load(LANG_PATH + currLanguage);
        app.installTranslator(&myappTranslator);
      }
    if (app.isRunning()) {
        QMessageBox::information(nullptr, QObject::tr("Programmstatus"),
                                 QObject::tr("Das Programm <b>")  + QString(APP_TITLE)
                                 + QObject::tr("</b> wurde bereits gestartet. "));
        return  !app.sendMessage(QObject::tr("Das Programm <b>")  + QString(APP_TITLE)
                                 + QObject::tr("</b> wurde bereits gestartet. "));

    }
    // Erstelle das Konfigurationsverzeichnis (&HOME/.simurun).
    // Create the directory for the config files (&HOME/.simurun).
    if(confDir.exists(APP_CONFIG_PATH) == false)
      {
        confDir.mkdir(APP_CONFIG_PATH);
      }


    // This is needed for the new configfile path!
    // Move an existing simurun2.conf from $HOME/.simurun2 to $HOME/.simurun
    if(confFile.exists(QDir::homePath()+"/.simurun2/simurun2.conf") == true
       and confFile.exists(APP_CONFIG_PATH + APP_NAME + ".conf") == false)
      {
        confFile.copy(QDir::homePath()+"/.simurun2/simurun2.conf",
                      APP_CONFIG_PATH + APP_NAME + ".conf" );
        confFile.copy(QDir::homePath()+"/.simurun2/read_version.sh",
                      APP_CONFIG_PATH + "read_version.sh" );

//        confFile.setFileName(QDir::homePath()+"/.config/"+ APP_NAME + ".conf");
//        confFile.remove();
      }

    // Überprüfe ob es ein installiertes Simutrans gibt!
    // Falls nicht, starte die Installation.
    // Check for an installed simutrans game.
    // Ifnot run the setup.
    if(confFile.exists(APP_CONFIG_PATH + "/" + APP_NAME + ".conf") == true)
      {
        QString gameDir;

        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans_Starter");
        gameDir = simurun_conf.value("GamePath").toString();
        simurun_conf.endGroup();

        if(gameDir.isEmpty())
          {
            SetupGame setup_start;
            setup_start.show();
            return app.exec();
          }
        else
          {
            simurun2 simurun_start;
            simurun_start.show();
            return app.exec();
          }

      }
    else
      {
        SetupGame setup_start;
        setup_start.show();
        return app.exec();
      }
  } // main end

//--- main.cpp end ---
