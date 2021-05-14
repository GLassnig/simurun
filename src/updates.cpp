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

#include "updates.h"

Updates::Updates(QWidget *parent) : QDialog(parent)
  {
  setupUi(this);
  setFixedSize(340, 280);
  setWindowTitle(tr("%1 - Updatecheck").arg(APP_TITLE));
//  setIcons();

  txt_Currend_Version->setText(APP_VERSION);


  QDir::setCurrent(QDir::homePath());
  }

void Updates::changeEvent(QEvent *e)
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

void Updates::on_cLB_Show_clicked()
  {
    open_Download_Page();
  }

void Updates::on_cLB_Download_clicked()
  {
    download_Source();
  }

void Updates::get_Update_Information()
  {

  }

void Updates::download_Source()
  {

  }

void Updates::open_Download_Page()
  {
    QString openWebSite = "http://www.e67-its.de/de/osp-de/simurun-de.html";
    QDesktopServices::openUrl(openWebSite);
  }

//QPixmap Updates::setThemeIcon(QString iconName)
//  {
//    KIconLoader *myIconLoader = new KIconLoader;

//    QPixmap setThemeIcon = myIconLoader->loadIcon(iconName,
//                                                  KIconLoader::Desktop,
//                                                  KIconLoader::SizeMedium,
//                                                  KIconLoader::DefaultState);
//    return setThemeIcon;
//  }

//void Updates::setIcons()
//  {
//    setWindowIcon(setThemeIcon("system-software-update"));
//  }
