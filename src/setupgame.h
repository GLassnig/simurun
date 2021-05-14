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

#ifndef SETUPGAME_H
#define SETUPGAME_H

#include <QLineEdit>
#include <QProgressBar>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QWizard>
#include <QMetaObject>

/* KDE Headers */
//#include <KDE/KIconLoader>

class QCheckBox;
class QGroupBox;
class QLabel;
//class QLineEdit;
class QRadioButton;
class QPushButton;
class QProgressBar;
class QComboBox;

class SetupGame : public QWizard {
    Q_OBJECT

  public:
    enum { Page_Intro, Page_Install, Page_Include, Page_InstallPakset,
           Page_UpdateGame,Page_RemovePakset,
           Page_RemoveSimutrans, Page_Finish };

    SetupGame(QWidget *parent = nullptr);

    void accept();

  private:
//    KIconLoader *myIconLoader;
  };

class IntroPage : public QWizardPage {
    Q_OBJECT

  public:
    IntroPage(QWidget *parent = 0);
    int nextId() const;

  private:
    QLabel *einleitungLabel;
    QRadioButton *installRadioButton;
    QRadioButton *includeRadioButton;
    QRadioButton *installPaksetRadioButton;
    QRadioButton *updateGameRadioButton;
    QRadioButton *removePaksetRadioButton;
    QRadioButton *removeSimutransRadioButton;
  };

class InstallPage : public QWizardPage {
    Q_OBJECT

  public:
    InstallPage(QWidget *parent = 0);
    int nextId() const;
    void chooseArchive();

    QString setArchive;
    QString setArchiveBase;
    QString setArchivePak;
    QString initialName;

  private:
    QLabel *infoLabel;
    QLabel *targetLabel;
    QLineEdit *targetLineEdit;
    QPushButton *targetPushButton;
    QLabel *gameBasepakLabel;
    QLabel *gamePaksetLabel;
    QLineEdit *gameBasepakLineEdit;
    QLineEdit *gamePaksetLineEdit;
    QPushButton *gameBasepakPushButton;
    QPushButton *gamePaksetPushButton;
    QProgressBar *installFortschritt;
//    KIconLoader *myIconLoader;

  private slots:
    void on_targetPushButton_clicked();
    void on_gameBasepakPushButton_clicked();
    void on_gamePaksetPushButton_clicked();
    void installButtonClicked();
  };

class IncludePage : public QWizardPage {
    Q_OBJECT

  public:
    IncludePage(QWidget *parent = 0);
    int nextId() const;

  private:
    QLabel *infoLabel;
    QLabel *gameLabel;
    QLineEdit * gameLineEdit;
    QPushButton *gamePushButton;
//    KIconLoader *myIconLoader;

  private slots:
    void on_gamePushButton_clicked();
  };

class InstallPaksetPage : public QWizardPage {
    Q_OBJECT

  public:
    InstallPaksetPage(QWidget *parent = 0);
    int nextId() const;
    void initializePage();

    QString setArchive;

  private:
    QLabel *infoLabel;
    QLabel *paksetLabel;
    QLabel *choosePathLabel;
    QLineEdit *choosePathEdit;
    QPushButton *choosePathButton;
    QLineEdit *paksetLineEdit;
    QPushButton *paksetPushButton;
    QProgressBar *installFortschritt;
//    KIconLoader *myIconLoader;

  private slots:
    void on_choosePathButton_clicked();
    void on_paksetPushButton_clicked();
    void installButtonClicked();
  };

class UpdateGamePage : public QWizardPage {
    Q_OBJECT

  public:
    UpdateGamePage(QWidget *parent = 0);
    int nextId() const;
    void initializePage();

    QString setArchive;

  private:
    QLabel *infoLabel;
    QLabel *baseLabel;
    QLineEdit *baseLineEdit;
    QPushButton *basePushButton;
    QProgressBar *installFortschritt;
    QLabel *choosePathLabel;
    QLineEdit *choosePathEdit;
    QPushButton *choosePathButton;
//    KIconLoader *myIconLoader;

  private slots:
    void on_choosePathButton_clicked();
    void on_basePushButton_clicked();
    void installButtonClicked();
  };

class RemovePaksetPage : public QWizardPage {
    Q_OBJECT

  public:
    RemovePaksetPage(QWidget *parent = 0);
    int nextId() const;
    void initializePage();

  private:
    QLabel *infoLabel;
    QLabel *paksetLabel;
    QComboBox *paksetComboBox;
    QPushButton *removePushButton;
    QProgressBar *installFortschritt;
//    KIconLoader *myIconLoader;

  private slots:
    void on_removePushButton_clicked();
  };

class RemoveSimutransPage : public QWizardPage {
    Q_OBJECT

  public:
    RemoveSimutransPage(QWidget *parent = 0);
    int nextId() const;
    QString gamePathremove;

  private:
    QLabel *infoLabel;
    QLabel *nameLabel;
    QLabel *gameLabel;
    QPushButton *rmSimutransPushButton;
    QProgressBar *installFortschritt;
//    KIconLoader *myIconLoader;

  private slots:
    void on_rmSimutransPushButton_clicked();
  };

class FinishPage : public QWizardPage {
    Q_OBJECT

  public:
    FinishPage(QWidget *parent = 0);
    int nextId() const;
    void initializePage();

  private:
    QLabel *gameVersionLabel;
    QLabel *gamePathLabel;
    QLabel *pakSetsLabel;
//    KIconLoader *myIconLoader;
  };

#endif // SETUPGAME_H
