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

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "ui_imageview.h"
#include "definitionen.h"

#include <QDialog>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QImage>
#include <QImageWriter>
#include <QPixmap>

/* KDE Headers */
//#include <KDE/KIconLoader>
//#include <KDE/KDialog>

namespace Ui
  {
    class ImageView;
  }

class ImageView : public QDialog, private Ui::ImageView {
    Q_OBJECT

  public:
    ImageView(QWidget *parent = nullptr);

  private slots:
    void on_gimpButton_clicked();
    void on_imageView_currentRowChanged(int currentRow);
    void on_imageView_currentItemChanged(QListWidgetItem* current,
                                         QListWidgetItem* previous);
    void saveAsImage();
    void fullScrViewer();
    void deleteImage();
    void closeViewer();
    void windowViewer();

  private:
    void createSlots();
    void readAsImage();
//    void setIcons();
//    QPixmap setThemeIcon(QString iconName);
    Ui::ImageView *m_ui;
  };

#endif // IMAGEVIEW_H