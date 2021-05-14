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

#ifndef SIMURUN_H
#define SIMURUN_H

/* KDE Headers */
//#include <KDE/KAction>
//#include <KDE/KMenu>
//#include <KDE/KIcon>
//#include <KDE/KIconLoader>
//#include <KDE/KHelpMenu>
//#include <KDE/KMessageBox>

/*
 * Qt Headers
 */
#include <QDesktopServices>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QListWidgetItem>

#include "globfunction.h"
#include "definitionen.h"
#include "about.h"
#include "setupgame.h"
#include "imageview.h"
#include "configureclass.h"
#include "updates.h"
#include "helpbrowser.h"
#include "parse_xml_local.h"
#include "parse_xml_net.h"

#include "ui_simurun.h"

class QPushButton;

class simurun2 : public QMainWindow, private Ui::simurun2Class {
  Q_OBJECT

public:
  simurun2(QWidget *parent = nullptr);
  ~simurun2();

private:
  void writeSettings();
  void readSettings();
  void loadPaksets();
  void runGameAuto();
  void showAboutKDE();
  void run_simutrans(QString arguments);
  void loadGameType();
  void createSlots();
//  void setIcons();
//  bool connectSQLiteDB();
//  QPixmap setThemeIcon(QString iconName);

  bool parseGameXml;
  bool readGameFiles;

  QString gamePath;

  QString gstartArguments;

  QString strI64;
  QString strI32;
  QString save_game_info;
  QString simutransTag;

  QString currentTag;
  QString serverStr;
  QString paksetStr;
  QString descStr;
  QString screenSize;

  // KHelpMenu *mHelpMenu;

  QMessageBox *netConMessage;

  QXmlStreamReader readLocalXml;
  QXmlStreamReader readNetXml;

  protected:
  virtual void changeEvent(QEvent *e);

  private slots:
  void ueberQt();
  void loadGame();
  void spielLadenRowChanged();
  void helpBrowser();
  void ueberSimurun();
  void newGame();
  void simurunExit();
  void deleteGame();
  void loadSettings();
  void loadSavedGames();

  void on_comboBoxPakSets_activated(QString);
  void on_comboBoxPakSets_currentIndexChanged(const QString);
  void on_rbNetzSpiel_clicked();
  void on_rbLokalesSpiel_clicked();
  void on_lastSavedGameCheckBox_clicked();
  void on_lastSavedGameCheckBox_clicked(bool checked);
  void on_musikCheckBox_clicked();
  void on_soundCheckBox_clicked();
  void on_checkBoxFullscreen_clicked();
  void on_actionUpdateSimutrans_triggered();
  void on_actionSimutransHomepage_triggered();
  void on_actionSimutransWiki_triggered();
  void on_actionSimutransForum_triggered();
  void on_actionScreenshots_triggered();
  void on_actionSimutrans_triggered();
  void on_actionSS2_Update_triggered();
};

#endif // SIMURUN_H
