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

#ifndef CONFIGURECLASS_H
#define CONFIGURECLASS_H

/* KDE Headers */
//#include <KDE/KIconLoader>

#include <QDateTime>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "ui_configureclass.h"
#include "globfunction.h"
#include "definitionen.h"
#include "languagetools.h"
//#include "networkgame.h"

class ConfigureClass : public QWidget, private Ui::ConfigureClass
  {
    Q_OBJECT

  public:
    explicit ConfigureClass(QWidget *parent = nullptr);
    void createLanguageMenu();
    QString contentVersion, contentPaksets;

  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_sbAbdeckungen_valueChanged(int wert);
    void on_sbKfzLebensdauer_valueChanged(int wert);
    void on_sbKfzLevel_valueChanged(int wert);
    void on_spinBoxMinFactorySpacing_valueChanged(int wert);
    void on_sbRadius_valueChanged(int wert);
    void on_sbMinCities_valueChanged(int wert);
    void on_sbMaxCities_valueChanged(int wert);
    void on_tabMain_currentChanged(int index);
    void on_tabSettings_currentChanged(int index);
    void on_pbSchliessen_clicked();
    void on_pbSpeichern_clicked();
    void on_cbBeginner_currentIndexChanged(int index);
    void on_sbDelay_valueChanged(int wert);
    void on_sbUpdates_valueChanged(int wert);
    void on_cbPakSets_currentIndexChanged(QString);
    void on_pbDafault_clicked();
    void on_chb_2_stateChanged(int arg1);
    void on_chb_3_stateChanged(int arg1);
    void on_chb_4_stateChanged(int arg1);
    void on_chb_5_stateChanged(int arg1);
    void on_chb_6_stateChanged(int arg1);
    void on_chb_7_stateChanged(int arg1);
    void on_chb_8_stateChanged(int arg1);
    void on_chb_9_stateChanged(int arg1);
    void on_chb_10_stateChanged(int arg1);
    void on_LanguageAppGroup_clicked(bool checked);
    void on_sbLandStrLaenge_valueChanged(int arg1);
    void on_sbAbstRath_valueChanged(int arg1);
    void on_chbStadtStr_2_stateChanged(int arg1);
    void on_chbStadtStr_3_stateChanged(int arg1);
    void on_chbStadtStr_4_stateChanged(int arg1);
    void on_chbStadtStr_5_stateChanged(int arg1);
    void on_chbStadtStr_6_stateChanged(int arg1);
    void on_chbStadtStr_7_stateChanged(int arg1);
    void on_chbStadtStr_8_stateChanged(int arg1);
    void on_chbStadtStr_9_stateChanged(int arg1);
    void on_chbStadtStr_10_stateChanged(int arg1);
    void on_chbLandStr_2_stateChanged(int arg1);
    void on_chbLandStr_3_stateChanged(int arg1);
    void on_chbLandStr_4_stateChanged(int arg1);
    void on_chbLandStr_5_stateChanged(int arg1);
    void on_chbLandStr_6_stateChanged(int arg1);
    void on_chbLandStr_7_stateChanged(int arg1);
    void on_chbLandStr_8_stateChanged(int arg1);
    void on_chbLandStr_9_stateChanged(int arg1);
    void on_chbLandStr_10_stateChanged(int arg1);
    void on_pbGenerateConfig_clicked();
    void on_chb_Ziele_2_stateChanged(int arg1);
    void on_chb_Ziele_3_stateChanged(int arg1);
    void on_chb_Ziele_4_stateChanged(int arg1);
    void on_chb_Ziele_5_stateChanged(int arg1);
    void on_chb_Ziele_6_stateChanged(int arg1);
    void on_chb_Ziele_7_stateChanged(int arg1);
    void on_chb_Ziele_8_stateChanged(int arg1);
    void on_chb_Ziele_9_stateChanged(int arg1);
    void on_chb_Ziele_10_stateChanged(int arg1);
    void on_cbServerAnnounce_currentIndexChanged(int index);
    void on_pbNetAdd_clicked();
    void on_pbNetEdit_clicked();
    void on_pbNetDelete_clicked();
    void on_txt_Server_textChanged(const QString &arg1);
    void on_serverTableWidget_cellClicked();

    void on_pbAendern_clicked();

  private:
    void createSlots();
    void loadValues();
    void loadRoadTypes();
    void loadDefaultValues();
    void readSettings();
    void writeSettings();
    void readSimutransSettings();
    void writeSimutransSettings();
    void executeChanges();
    void executeSlots();
    void createSimuconf();
//    void setIcons();
    void writeServerXml();
    void readServerXml();
    void addXmlItems(QString server, QString pakset, QString desc);

//    QPixmap setThemeIcon(QString iconName);

    QString gamePath;
    QString paksetActive;
    QString currentTag;
    QString serverStr;
    QString paksetStr;
    QString descStr;

    LanguageTools * languages;
    QMap<QString, QString> * fileToNiceName;

    QXmlStreamReader readNetGamesXml;
    QXmlStreamWriter writeNetGamesXml;
  };

#endif // CONFIGURECLASS_H
