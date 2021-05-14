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

#ifndef UPDATES_H
#define UPDATES_H


//#include <KDE/KIcon>
//#include <KDE/KIconLoader>

//#include <QtNetwork>
//#include <QNetworkAccessManager>
#include <QDesktopServices>
#include <QUrl>

class QProgressDialog;
class QPushButton;
class QSslError;
class QNetworkReply;

#include "ui_updates.h"
#include "definitionen.h"

class Updates : public QDialog, private Ui::Updates
  {
    Q_OBJECT

  public:
    explicit Updates(QWidget *parent = nullptr);

    void startRequest(QUrl url);

  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_cLB_Show_clicked();
    void on_cLB_Download_clicked();
//    void downloadFile();
//    void cancelDownload();
//    void httpFinished();
//    void httpReadyRead();
//    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
//    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
//    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);

  private:
    void get_Update_Information();
    void download_Source();
    void open_Download_Page();
//    void setIcons();

//    QPixmap setThemeIcon(QString iconName);
    QUrl url;
//    QNetworkAccessManager qnam;
//    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
  };

#endif // UPDATES_H
