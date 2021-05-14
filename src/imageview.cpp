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

#include "imageview.h"

ImageView::ImageView(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::ImageView) {

    m_ui->setupUi(this);
    m_ui->retranslateUi(this);
    m_ui->windowModeButton->setVisible(false);
    m_ui->imageView->setFocus();


    setWindowTitle(tr("%1 - %2 - "
                          "Screenshot Betrachter"
                          ).arg(APP_TITLE).arg(APP_VERSION));

    setMinimumSize(700, 450);
//    setIcons();

    // Don't show me the dafault Buttons.
    // Die Standard Buttons nicht anzeigen.
//    KDialog::showButton(KDialog::Cancel, false);
//    KDialog::showButton(KDialog::Ok, false);

//    KDialog::setContentsMargins(0,0,0,0);
//    KDialog::setMainWidget(m_ui->myAppWidget);

    QFile gimpBin;

    if (gimpBin.exists("/usr/bin/gimp") == true) {
        m_ui->gimpButton->setVisible(true);
      } else {
        m_ui->gimpButton->setVisible(false);
      }

    m_ui->saveAsButton->setVisible(false);

    createSlots();
    readAsImage();
  }

void ImageView::createSlots() {
    connect(m_ui->closeButton, SIGNAL(clicked()), this, SLOT(closeViewer()));
    connect(m_ui->windowModeButton, SIGNAL(clicked()),
            this, SLOT(windowViewer()));
    connect(m_ui->fullscreenButton, SIGNAL(clicked()),
            this, SLOT(fullScrViewer()));
    connect(m_ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteImage()));
    connect(m_ui->saveAsButton, SIGNAL(clicked()), this, SLOT(saveAsImage()));
  }

void ImageView::closeViewer() {
    this->close();
  }

void ImageView::fullScrViewer() {
    showFullScreen();
    m_ui->fullscreenButton->setVisible(false);
    m_ui->windowModeButton->setVisible(true);
  }

void ImageView::windowViewer() {
    showNormal();
    showMaximized();
    m_ui->fullscreenButton->setVisible(true);
    m_ui->windowModeButton->setVisible(false);
  }

void ImageView::saveAsImage() {
    QImageWriter saveImageAs;
    QImage imageName;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    QDir::homePath() +
                                                    "/simutrans/screenshot/" +
                                                    m_ui->imageView->currentItem()->text(),
                                                    tr("Bitmap (*.bmp)"));
    imageName.save(fileName);
    saveImageAs.setFileName(fileName);
    saveImageAs.write(imageName);
  }

void ImageView::deleteImage() {
    QFile delImage;
    QMessageBox msgBox;

    msgBox.setWindowTitle(tr("%1 - %2"
                                 ).arg(APP_TITLE).arg(APP_VERSION));
    msgBox.setText(tr("<b>Wollen Sie den gespeichertern"
                          " Screenshot wirklich löschen?</b>"));
    msgBox.setInformativeText("<b>" + m_ui->imageView->currentItem()->text() +
                              tr("</b> wird unwiederbringlich gelöscht!"
                                     ));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
        delImage.remove(QDir::homePath() + "/simutrans/screenshot/" +
                        m_ui->imageView->currentItem()->text());
        m_ui->imageView->takeItem(m_ui->imageView->currentRow());
        break;
      case QMessageBox::No:
        msgBox.close();
        break;
      default:
        // should never be reached
        break;
      }
  }

void ImageView::readAsImage() {

    QDir dir(QDir::homePath() + "/simutrans/screenshot/");
    QStringList filter;
    filter << "sim*.bmp";
    QStringList fileNames = dir.entryList(filter);

    m_ui->imageView->clear();
    for (int i = 0; i < (int)fileNames.size(); ++i) {
        m_ui->imageView->addItem(fileNames[i]);
      }
  }

void ImageView::on_imageView_currentRowChanged(int currentRow) {
    if (currentRow == -1){
        m_ui->screenshotLabel->clear();
        m_ui->deleteButton->setEnabled(false);
        m_ui->saveAsButton->setEnabled(false);
        m_ui->gimpButton->setEnabled(false);
      } else {
        m_ui->screenshotLabel->setPixmap(QPixmap(QDir::homePath() +
                                                 "/simutrans/screenshot/" +
                                                 m_ui->imageView->currentItem()->text()));
      }
  }

void ImageView::on_imageView_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
  {
    if (!current)
      current = previous;

    m_ui->deleteButton->setEnabled(true);
    m_ui->saveAsButton->setEnabled(true);
    m_ui->gimpButton->setEnabled(true);
  }

void ImageView::on_gimpButton_clicked()
  {
    QProcess *gimpProcess = new QProcess;
    gimpProcess->start("gimp " + QDir::homePath() +
                       "/simutrans/screenshot/" +
                       m_ui->imageView->currentItem()->text());
  }

//QPixmap ImageView::setThemeIcon(QString iconName)
//  {
//    KIconLoader *myIconLoader = new KIconLoader;

//    QPixmap setThemeIcon = myIconLoader->loadIcon(iconName,
//                                                  KIconLoader::Desktop,
//                                                  KIconLoader::SizeMedium,
//                                                  KIconLoader::DefaultState);
//    return setThemeIcon;

//  }

//void ImageView::setIcons()
//  {
//    setWindowIcon(setThemeIcon("camera-photo"));
//    m_ui->saveAsButton->setIcon(setThemeIcon("document-save-as"));
//    m_ui->deleteButton->setIcon(setThemeIcon("trash-empty"));
//    m_ui->gimpButton->setIcon(setThemeIcon("gimp"));
//    m_ui->fullscreenButton->setIcon(setThemeIcon("view-fullscreen"));
//    m_ui->windowModeButton->setIcon(setThemeIcon("view-restore"));
//    m_ui->closeButton->setIcon(setThemeIcon("application-exit"));
//  }
