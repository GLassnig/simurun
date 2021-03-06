﻿/******************************************************************************
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

#include "about.h"

About_SS2::About_SS2(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  retranslateUi(this);
  setFixedSize(530, 400);

  tabWidget->setCurrentIndex (0);

  lblAppTitel->setText(tr(APP_TITLE));
  lblAppVersion->setText(tr("Version: ") + tr(APP_VERSION));

  QString copyright = tr("(c) 2007 - ") + QDate::currentDate().toString("yyyy") + tr(" by G. Lassnig");
  label_Copyright->setText(copyright);


  loadInfos();
}

void About_SS2::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      retranslateUi(this);
      break;
    default:
      break;
    }
}

void About_SS2::loadInfos() {
  QString contentVersion, contentPath, contentPaksetActive, contentPaksets;

  // Lese Simutrans Version ein.
  // Read the simutrans version.
  QFile fileVersion(APP_CONFIG_PATH + "SimutransVersion.lst");
  fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream inVersion(&fileVersion);
  contentVersion = inVersion.readLine();

  // Zeige Installationspfad und aktives Pakset an.
  QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                         APP_NAME + ".conf", QSettings::NativeFormat);
  contentPath = simurun_conf.value("Simutrans_Starter/GamePath").toString();
  contentPaksetActive = simurun_conf.value(
                          "Simutrans/pak_file_path").toString();

  // Lese Paksets ein.
  QDir dir(contentPath + "/", "*pak*");
  QStringList filter;
  filter << "*pak*";
  QStringList dirNames = dir.entryList();

  for (int i = 0; i < (int)dirNames.size(); ++i) {
      contentPaksets += dirNames[i] + "<br>";
    }

  versionInfoLabel->setText("<b>" + contentVersion + "</b>");
  pfadInfoLabel->setText("<b>" + contentPath + "</b>");
  paksetInfoLabel->setText("<b>" + contentPaksetActive + "</b>");
  instPaksetsInfoLabel->setText("<b>" + contentPaksets + "</b>");
}
