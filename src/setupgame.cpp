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

#include <QtGui>
#include <QFileInfo>

#include "definitionen.h"
#include "setupgame.h"
#include "simurun.h"
#include "globfunction.h"

SetupGame::SetupGame(QWidget *parent) : QWizard(parent) {
    setPage(Page_Intro, new IntroPage);
    setPage(Page_Install, new InstallPage);
    setPage(Page_Include, new IncludePage);
    setPage(Page_InstallPakset, new InstallPaksetPage);
    setPage(Page_UpdateGame, new UpdateGamePage);
    setPage(Page_RemovePakset, new RemovePaksetPage);
    setPage(Page_RemoveSimutrans, new RemoveSimutransPage);
    setPage(Page_Finish, new FinishPage);

    setStartId(Page_Intro);

//    myIconLoader = new KIconLoader;

    setWindowIcon(QIcon(":/images/images/icons/wizard.png"));

    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/images/simurun2.png"));

    setWindowTitle(tr("Simutrans Starter - Setup-Assistent"));
    SetupGame::setMinimumSize(600, 360);
  }

void SetupGame::accept() {
    this->close();

  }

IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));

    setPixmap(QWizard::WatermarkPixmap,
              QPixmap(":/images/images/simutrans_starter_logo-rund.png"));

    einleitungLabel = new QLabel(tr("Der Setup-Assistent wird Sie nun "
                                        "durch die Installation bzw. Einbindung der "
                                        "<b>Simutrans Wirtschaftssimulation</b> begleiten.\n\n"
                                        " "));
    einleitungLabel->setWordWrap(true);

    installRadioButton = new QRadioButton(tr("&Simutrans installieren"));
    includeRadioButton = new QRadioButton(tr("Simutransinstallation &einbinden"));
    installPaksetRadioButton = new QRadioButton(tr("&Pakset installieren/aktuallisieren"));
    updateGameRadioButton = new QRadioButton(tr("Simutrans Basispaket &aktuallisieren"));
    removePaksetRadioButton = new QRadioButton(tr("Pa&kset deinstallieren"));
    removeSimutransRadioButton = new QRadioButton(tr("Simutrans &deinstallieren"));

    QString gamePath;
    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gamePath = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    if(QFile::exists(APP_CONFIG_PATH + APP_NAME + ".conf") == false
       or gamePath.isEmpty())
      {
        installPaksetRadioButton->setEnabled(false);
        updateGameRadioButton->setEnabled(false);
        removePaksetRadioButton->setEnabled(false);
        removeSimutransRadioButton->setEnabled(false);

        installRadioButton->setChecked(true);
      } else if(QFile::exists(APP_CONFIG_PATH + APP_NAME + ".conf")
                == true) {
        // Lese Simutrans Version ein.
        // Read the simutrans version.
        QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
        fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inVersion(&fileVersion);
        QString simuVersion = inVersion.readLine();

        einleitungLabel->setText(tr("Bitte wählen Sie eine Aktion aus."));

        setSubTitle(tr("Aktuelle Simutrans Version: ") + "<b>" + simuVersion +
                    "</b><br>" + tr("Programmpfad: ") +
                    "<b>" + gamePath + "</b>");

        installRadioButton->setEnabled(false);
        includeRadioButton->setEnabled(false);

        installPaksetRadioButton->setFocus();
        installPaksetRadioButton->setChecked(true);

      }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(einleitungLabel);
    layout->addWidget(installRadioButton);
    layout->addWidget(includeRadioButton);
    layout->addWidget(installPaksetRadioButton);
    layout->addWidget(updateGameRadioButton);
    layout->addWidget(removePaksetRadioButton);
    layout->addWidget(removeSimutransRadioButton);
    setLayout(layout);

  }

int IntroPage::nextId() const {
    if (installRadioButton->isChecked()) { return SetupGame::Page_Install; }
    if (includeRadioButton->isChecked()) { return SetupGame::Page_Include; }
    if (installPaksetRadioButton->isChecked()) {
        return SetupGame::Page_InstallPakset; }
    if (updateGameRadioButton->isChecked()) { return SetupGame::Page_UpdateGame; }
    if (removePaksetRadioButton->isChecked()) {
        return SetupGame::Page_RemovePakset; }
    if (removeSimutransRadioButton->isChecked()) {
        return SetupGame::Page_RemoveSimutrans; }
    return 0;
  }

InstallPage::InstallPage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Simutrans installieren"));

    infoLabel = new QLabel(tr("Bitte wählen Sie das Zielverzeichnis aus, "
                                  "in das Simutrans installiert werden soll. "
                                  "Sowie ein Simutrans Basispaket "
                                  "(simulinux-xxx-x.zip) und ein Pakset "
                                  "(z.B. pak64-xxx-x.zip, pak128-x-x-x_xxx-x.zip "
                                  "usw.). Wird Simutrans nicht im "
                                  "Benutzerverzeichnis installiert, sind "
                                  "root-Rechte erforderlich.<br>Bitte stellen Sie "
                                  "sicher, dass die zu installierenden Pakete "
                                  "kompatibel zueinander sind."));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);
    targetLabel = new QLabel(tr("&Installationsverzeichnis:"));
    targetLineEdit = new QLineEdit();
    targetLabel->setBuddy(targetLineEdit);

    targetPushButton = new QPushButton;
    QIcon icon2(":/images/images/icons/fileopen.png");
//    myIconLoader = new KIconLoader;

    targetPushButton->setIcon(icon2);

    gameBasepakLabel = new  QLabel(tr("&Basispaket auswählen:"));;
    gamePaksetLabel = new  QLabel(tr("&Pakset auswählen:"));;
    gameBasepakLineEdit = new  QLineEdit;
    gamePaksetLineEdit = new  QLineEdit;

    gameBasepakLabel->setBuddy(gameBasepakLineEdit);
    gamePaksetLabel->setBuddy(gamePaksetLineEdit);

    gameBasepakPushButton = new  QPushButton;
    gamePaksetPushButton = new  QPushButton;
    icon2.addPixmap(QPixmap(":/images/images/pak-file.png"));
    gameBasepakPushButton->setIcon(icon2);
    gamePaksetPushButton->setIcon(icon2);

    installFortschritt = new QProgressBar;

    registerField("target*", targetLineEdit);
    registerField("gameBasepak*", gameBasepakLineEdit);
    registerField("gamePakset*", gamePaksetLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0, 3, 3);
    layout->addWidget(targetLabel, 3, 0);
    layout->addWidget(targetLineEdit, 3, 1);
    layout->addWidget(targetPushButton, 3, 2);
    layout->addWidget(gameBasepakLabel, 4, 0);
    layout->addWidget(gameBasepakLineEdit, 4, 1);
    layout->addWidget(gameBasepakPushButton, 4, 2);
    layout->addWidget(gamePaksetLabel, 5, 0);
    layout->addWidget(gamePaksetLineEdit, 5, 1);
    layout->addWidget(gamePaksetPushButton, 5, 2);
    layout->addWidget(installFortschritt, 6, 0, 3, 0);
    setLayout(layout);

    connect(targetPushButton, SIGNAL(clicked()),
            this, SLOT(on_targetPushButton_clicked()));
    connect(gameBasepakPushButton, SIGNAL(clicked()),
            this, SLOT(on_gameBasepakPushButton_clicked()));
    connect(gamePaksetPushButton, SIGNAL(clicked()),
            this, SLOT(on_gamePaksetPushButton_clicked()));
  }

void InstallPage::on_targetPushButton_clicked() {
    QFileDialog TargetDir_Open;

    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    initialName =  TargetDir_Open.getExistingDirectory(this,
                                                       tr("Verzeichnis wählen"),
                                                       initialName,
                                                       QFileDialog::ShowDirsOnly);
    initialName += "/simutrans";
    targetLineEdit->setText(initialName);
  }

void InstallPage::on_gameBasepakPushButton_clicked() {
    chooseArchive();
    setArchiveBase = setArchive;
    gameBasepakLineEdit->setText(setArchiveBase);
  }

void InstallPage::on_gamePaksetPushButton_clicked() {
    chooseArchive();
    setArchivePak = setArchive;
    gamePaksetLineEdit->setText(setArchivePak);

    if (setArchiveBase.isEmpty() == false) {
        if (initialName.isEmpty() == false) {
            if (setArchivePak.isEmpty() == false) {
                wizard()->setButtonText(QWizard::CustomButton3, tr("I&nstallieren"));
                wizard()->setOption(QWizard::HaveCustomButton3, true);
                connect(wizard(), SIGNAL(customButtonClicked(int)),
                        this, SLOT(installButtonClicked()));
              }
          }
      } else {
        wizard()->setOption(QWizard::HaveCustomButton3, false);
        disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                   this, SLOT(installButtonClicked()));
      }
  }

void InstallPage::installButtonClicked() {
    GlobFunction gF;

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    simurun_conf.setValue("GamePath", initialName);
    simurun_conf.endGroup();

    installFortschritt->setValue(25);

    gF.install_Archive(initialName, setArchiveBase);

    installFortschritt->setValue(50);

    gF.install_Archive(initialName, setArchivePak);

    installFortschritt->setValue(75);

    gF.read_simutrans_version_script(initialName);
    gF.read_simutrans_version();

    installFortschritt->setValue(100);

    wizard()->setOption(QWizard::HaveCustomButton3, false);
  }

void InstallPage::chooseArchive() {
    QFileDialog PakSetDir;
    QString initialName;
    initialName = setArchive;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    setArchive = PakSetDir.getOpenFileName(this,
                                           tr("Archiv wählen"),
										   initialName,
										   tr("Zip Archiv (*.zip);;7-Zip Archiv (*.7z);;GNU Zip Archiv (*.gz);;RAR Archiv (*.rar)"));
  }

int InstallPage::nextId() const {
    return SetupGame::Page_Finish;
  }

IncludePage::IncludePage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Simutrans einbinden"));

    infoLabel = new QLabel(tr("Bitte geben Sie das Verzeichnis an, "
                                  "in dem sich die ausführbare Simutransdatei "
                                  "befindet."));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);
    gameLabel = new QLabel(tr("&Installation auswählen:"));
    gameLineEdit = new QLineEdit;
    gameLabel->setBuddy(gameLineEdit);
    gamePushButton = new QPushButton;
//    myIconLoader = new KIconLoader;
    gamePushButton->setIcon(QIcon(":/images/images/icons/folder-blue.png"));

    registerField("gameLine*", gameLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0, 3, 3);
    layout->addWidget(gameLabel, 3, 0);
    layout->addWidget(gameLineEdit, 3, 1);
    layout->addWidget(gamePushButton, 3, 2);
    setLayout(layout);

    connect(gamePushButton, SIGNAL(clicked()),
            this, SLOT(on_gamePushButton_clicked()));

  }

void IncludePage::on_gamePushButton_clicked() {
    QFileDialog TargetDir_Open;
    QString initialName;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    initialName =  TargetDir_Open.getExistingDirectory(this,
                                                       tr("Verzeichnis wählen"),
                                                       initialName,
                                                       QFileDialog::ShowDirsOnly);
    gameLineEdit->setText(initialName);


    GlobFunction ReadPakSets;
    ReadPakSets.read_simutrans_version_script(initialName);
    ReadPakSets.read_simutrans_version();

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    simurun_conf.setValue("GamePath", initialName);
    simurun_conf.endGroup();

  }

int IncludePage::nextId() const {
    return SetupGame::Page_Finish;
  }

void InstallPaksetPage::initializePage() {
    paksetLineEdit->setEnabled(true);
    paksetPushButton->setEnabled(true);
    installFortschritt->reset();
  }

InstallPaksetPage::InstallPaksetPage(QWidget *parent) : QWizardPage(parent) {
    QIcon icon1, icon2;

    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Weiteres Pakset installieren"));

    infoLabel = new QLabel(tr("Bitte wählen Sie das Pakset aus, "
                                  "das Sie installieren möchten!"
                                  "<br><b>ACHTUNG:</b><br>Stellen Sie "
                                  "sicher, dass das zu installierende Pakset "
                                  "kompatibel zu Ihrer Simutrans Version ist. "
                                  "Gespeicherte Spielstände können unter "
                                  "umständen nicht mehr geladen werden.<br>"
                                  "Klicken Sie danach auf <b>Installieren</b>."));
    infoLabel->setWordWrap(true);

    choosePathLabel = new QLabel(tr("Simutrans &Verzeichnis:"));
    choosePathEdit = new QLineEdit;
    choosePathEdit->setToolTip(tr("Den Installationspfad ändern.<br>"
                                      "Der Pfad wird für gewöhnlich bei der "
                                      "Installation/Einbindung in die<br>"
                                      "<i>.config/simurun2.conf</i> geschrieben "
                                      "und wieder gelesen"
                                      ));
    choosePathButton = new QPushButton;
//    myIconLoader = new KIconLoader;
    choosePathButton->setIcon(QIcon(":/images/images/simutrans.png"));
    QSize iconSize(16, 16);
    choosePathButton->setIconSize(iconSize);
    choosePathButton->setToolTip(tr("Den Installationspfad ändern.<br>"
                                        "Der Pfad wird für gewöhnlich bei der "
                                        "Installation/Einbindung in die<br>"
                                        "<i>.config/simurun2.conf</i> geschrieben "
                                        "und wieder gelesen"));
    choosePathLabel->setBuddy(choosePathEdit);

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    choosePathEdit->setText(simurun_conf.value("GamePath").toString());
    simurun_conf.endGroup();

    paksetLabel = new QLabel(tr("&Pakset:"));
    paksetLineEdit = new QLineEdit;
    paksetPushButton = new QPushButton;
    icon2.addPixmap(QPixmap(":/images/images/pak-file.png"));
    paksetPushButton->setIcon(icon2);
    paksetLabel->setBuddy(paksetLineEdit);

    installFortschritt = new QProgressBar;
    installFortschritt->reset();




    registerField("pakset*", paksetLineEdit);
    //  registerField("choosePath*", choosePathEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0, 3, 0, Qt::AlignTop);
    layout->addWidget(choosePathLabel, 2, 0);
    layout->addWidget(choosePathEdit, 2, 1);
    layout->addWidget(choosePathButton, 2, 2);
    layout->addWidget(paksetLabel, 3, 0);
    layout->addWidget(paksetLineEdit, 3, 1);
    layout->addWidget(paksetPushButton, 3, 2);
    layout->addWidget(installFortschritt, 5, 0, 3, 0, Qt::AlignTop);
    setLayout(layout);

    connect(paksetPushButton, SIGNAL(clicked()),
            this, SLOT(on_paksetPushButton_clicked()));
    connect(choosePathButton, SIGNAL(clicked()),
            this, SLOT(on_choosePathButton_clicked()));

  }

void InstallPaksetPage::on_choosePathButton_clicked(){
    QFileDialog gamePath;
    QString initialName, gameDir;

    initialName = setArchive;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    gameDir = gamePath.getExistingDirectory(this, tr("Verzeichnis wählen"),
                                            initialName,
                                            QFileDialog::ShowDirsOnly);
    choosePathEdit->setText(gameDir);

  }

void InstallPaksetPage::on_paksetPushButton_clicked() {
    QFileDialog PakSetDir;
    QString initialName;

    initialName = setArchive;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    setArchive = PakSetDir.getOpenFileName(this,
                                           tr("Archiv wählen"),
										   initialName,
										   tr("Zip Archiv (*.zip);;7-Zip Archiv (*.7z);;GNU Zip Archiv (*.gz);;RAR Archiv (*.rar)"));
    paksetLineEdit->setText(setArchive);

    if (setArchive.isEmpty() == false) {
        if (initialName.isEmpty() == false) {
            wizard()->setButtonText(QWizard::CustomButton1, tr("I&nstallieren"));
            wizard()->setOption(QWizard::HaveCustomButton1, true);
            connect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(installButtonClicked()));
          }
      } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
        disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                   this, SLOT(installButtonClicked()));
      }
  }

void InstallPaksetPage::installButtonClicked() {
    GlobFunction gF;

    QString meldung, archiveInfo;
    QFileInfo fileInfo(setArchive);

    installFortschritt->reset();

    installFortschritt->setValue(50);

    gF.install_Archive(choosePathEdit->text() + "/", setArchive);

    installFortschritt->setValue(100);

    archiveInfo = fileInfo.fileName();
    meldung = "Das Pakset-Archiv <i>" + archiveInfo +
              "</i> wurde erfolgreich installiert!";

    gF.global_message(meldung);

    wizard()->setOption(QWizard::HaveCustomButton1, false);

    paksetLineEdit->setDisabled(true);
    paksetPushButton->setDisabled(true);
    choosePathEdit->setDisabled(true);
    choosePathButton->setDisabled(true);
  }

int InstallPaksetPage::nextId() const {
    return SetupGame::Page_Finish;
  }

void UpdateGamePage::initializePage() {
    baseLineEdit->setEnabled(true);
    basePushButton->setEnabled(true);
    installFortschritt->reset();
  }

UpdateGamePage::UpdateGamePage(QWidget *parent) : QWizardPage(parent) {
    QIcon icon, icon1;

    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Simutrans Basispaket aktuallisieren"));

    infoLabel = new QLabel(tr("Bitte wählen Sie das Simutrans Paket aus, "
                                  "das Sie installieren möchten.<br>"
                                  "<b>ACHTUNG:</b>"
                                  "<br> Stellen Sie vorher sicher, dass die neue "
                                  "Simutrans Version kompatibel zu den "
                                  "installierten PakSets ist. Unter Umständen kann "
                                  "es dazu kommen, dass Simutrans nicht mehr "
                                  "startet oder gespeicherte Spielstände nicht "
                                  "mehr geladen werden."));
    infoLabel->setWordWrap(true);

    choosePathLabel = new QLabel(tr("Simutrans &Verzeichnis:"));
    choosePathEdit = new QLineEdit;
    choosePathEdit->setToolTip(tr("Den Installationspfad ändern.<br>"
                                      "Der Pfad wird für gewöhnlich bei der "
                                      "Installation/Einbindung in die<br>"
                                      "<i>.config/simurun2.conf</i> geschrieben "
                                      "und wieder gelesen"
                                      ));
    choosePathButton = new QPushButton;
//    myIconLoader = new KIconLoader;
    QSize iconSize(16, 16);
    choosePathButton->setIconSize(iconSize);
    choosePathButton->setIcon(QIcon(":/images/images/simutrans.png"));
    choosePathButton->setToolTip(tr("Den Installationspfad ändern.<br>"
                                        "Der Pfad wird für gewöhnlich bei der "
                                        "Installation/Einbindung in die<br>"
                                        "<i>.config/simurun2.conf</i> geschrieben "
                                        "und wieder gelesen"));
    choosePathLabel->setBuddy(choosePathEdit);

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    choosePathEdit->setText(simurun_conf.value("GamePath").toString());
    simurun_conf.endGroup();

    baseLabel = new QLabel(tr("&Simutrans Basis Paket:"));
    baseLineEdit = new QLineEdit;
    basePushButton = new QPushButton;
    icon.addPixmap(QPixmap(":/images/images/pak-file.png"));
    basePushButton->setIcon(icon);

    installFortschritt = new QProgressBar;
    installFortschritt->reset();

    baseLabel->setBuddy(baseLineEdit);

    registerField("basePaket*", baseLineEdit);
    //  registerField("choosePath*", choosePathEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0, 3, 0, Qt::AlignTop);
    layout->addWidget(choosePathLabel, 2, 0);
    layout->addWidget(choosePathEdit, 2, 1);
    layout->addWidget(choosePathButton, 2, 2);
    layout->addWidget(baseLabel, 3, 0);
    layout->addWidget(baseLineEdit, 3, 1);
    layout->addWidget(basePushButton, 3, 2);
    layout->addWidget(installFortschritt, 5, 0, 3, 0, Qt::AlignTop);
    setLayout(layout);

    connect(basePushButton, SIGNAL(clicked()),
            this, SLOT(on_basePushButton_clicked()));
    connect(choosePathButton, SIGNAL(clicked()),
            this, SLOT(on_choosePathButton_clicked()));

  }

void UpdateGamePage::on_choosePathButton_clicked(){
    QFileDialog gamePath;
    QString initialName, gameDir;

    initialName = setArchive;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    gameDir = gamePath.getExistingDirectory(this, tr("Verzeichnis wählen"),
                                            initialName,
                                            QFileDialog::ShowDirsOnly);
    choosePathEdit->setText(gameDir);
  }

void UpdateGamePage::on_basePushButton_clicked() {
    QFileDialog BaseDir;
    QString initialName;

    initialName = setArchive;
    if(initialName.isEmpty()) {
        initialName = QDir::homePath();
      }

    setArchive = BaseDir.getOpenFileName(this,
                                         tr("Archiv wählen"),
										 initialName,
										 tr("Zip Archiv (*.zip);;7-Zip Archiv (*.7z);;GNU Zip Archiv (*.gz);;RAR Archiv (*.rar)"));
    baseLineEdit->setText(setArchive);

    if (setArchive.isEmpty() == false) {
        if (initialName.isEmpty() == false) {
            wizard()->setButtonText(QWizard::CustomButton1, tr("I&nstallieren"));
            wizard()->setOption(QWizard::HaveCustomButton1, true);
            connect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(installButtonClicked()));
          }
      } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
        disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                   this, SLOT(installButtonClicked()));
      }
  }

void UpdateGamePage::installButtonClicked() {
    GlobFunction gF;

    QString gameDir, meldung, archiveInfo;
    QFileInfo fileInfo(setArchive);

    installFortschritt->reset();

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gameDir = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    installFortschritt->setValue(50);

    gF.install_Archive(choosePathEdit->text() + "/", setArchive);

    installFortschritt->setValue(100);

    archiveInfo = fileInfo.fileName();
    meldung = "Das Simutrans Paket: <i>" + archiveInfo +
              "</i> wurde erfolgreich installiert!";

    gF.global_message(meldung);

    wizard()->setOption(QWizard::HaveCustomButton1, false);

    baseLineEdit->setDisabled(true);
    basePushButton->setDisabled(true);
    choosePathEdit->setDisabled(true);
    choosePathButton->setDisabled(true);

  }

int UpdateGamePage::nextId() const {
    return SetupGame::Page_Finish;
  }

void RemovePaksetPage::initializePage() {
    QString gamePath;
    paksetComboBox->clear();
    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gamePath = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    // Lese Paksets in die Combobox ein.
    // Fill ComboBox with the paksets
    QDir dir(gamePath + "/", "*pak*");
    QStringList filter;
    filter << "*pak*";

    QStringList dirNames = dir.entryList();

    for (int i = 0; i < (int)dirNames.size(); ++i) {
        paksetComboBox->addItem(dirNames[i]);
      }
  }

RemovePaksetPage::RemovePaksetPage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Simutrans Pakset entfernen"));

    infoLabel = new QLabel(tr("Bitte wählen Sie das  zu entfernende "
                                  "Pakset aus der Combobox aus. Das "
                                  "Pakset wird dann aus dem Simutrans "
                                  "Verzeichnis gelöscht."));
    infoLabel->setWordWrap(true);
    paksetLabel = new QLabel(tr("Installierte &Paksets:"));
    paksetComboBox = new QComboBox;

//    myIconLoader = new KIconLoader;
    removePushButton = new QPushButton;
    removePushButton->setText(tr("Pakset &löschen"));
    removePushButton->setIcon(QIcon(":/images/images/icons/user-trash.png"));

    installFortschritt = new QProgressBar;

    paksetLabel->setBuddy(paksetComboBox);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(infoLabel, 0, 0, 3, 0, Qt::AlignTop);
    layout->addWidget(paksetLabel, 3, 0);
    layout->addWidget(paksetComboBox, 3, 1);
    layout->addWidget(removePushButton, 3, 2);
    layout->addWidget(installFortschritt, 4, 0, 3, 0, Qt::AlignBottom);
    setLayout(layout);

    connect(removePushButton, SIGNAL(clicked()),
            this, SLOT(on_removePushButton_clicked()));

  }

void RemovePaksetPage::on_removePushButton_clicked() {
    QString removePak, gamePath;
    QMessageBox msgBox;
    QProcess *rmPak = new QProcess;

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);

    simurun_conf.beginGroup("Simutrans_Starter");
    gamePath = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    removePak = gamePath + "/" + paksetComboBox->currentText();

    msgBox.setText(tr("<b>Wollen Sie das ausgewählte"
                          " Pakset wirklich löschen?</b>"));
    msgBox.setInformativeText(tr("Die Daten werden "
                                     "unwiederbringlich gelöscht!"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Yes:
        installFortschritt->setValue(25);
        rmPak->execute("rm -vfr " + removePak);
        if (rmPak->exitStatus() == false) {
            installFortschritt->setValue(50);
            initializePage();
            installFortschritt->setValue(100);
          }
        break;
      case QMessageBox::No:
        msgBox.close();
        break;
      default:
        // should never be reached
        break;
      }
  }

int RemovePaksetPage::nextId() const {
    return -1; //SetupGame::Page_Finish;

  }

RemoveSimutransPage::RemoveSimutransPage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Simutrans entfernen"));

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gamePathremove = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    infoLabel = new QLabel(tr("Simutrans wird mit allen Daten "
                                  "(gespeicherte Spielstände, Screenshots) "
                                  "gelöscht!"));
    infoLabel->setWordWrap(true);

    nameLabel = new QLabel(tr("Simutrans löschen in:"));
    gameLabel = new QLabel("<b>" + gamePathremove + "/<b>");

//    myIconLoader = new KIconLoader;
    rmSimutransPushButton = new QPushButton;
    rmSimutransPushButton->setText(tr("Simutrans &löschen"));
    rmSimutransPushButton->setIcon(QIcon(":/images/images/icons/user-trash.png"));

    installFortschritt = new QProgressBar;

    QGridLayout *meinlayout = new QGridLayout;
    meinlayout->addWidget(infoLabel, 0, 0, 3, 0, Qt::AlignTop);
    meinlayout->addWidget(nameLabel, 3, 0);
    meinlayout->addWidget(gameLabel, 3, 1);
    meinlayout->addWidget(rmSimutransPushButton, 3, 2);
    meinlayout->addWidget(installFortschritt, 4, 0, 3, 0, Qt::AlignBottom);
    setLayout(meinlayout);

    connect(rmSimutransPushButton, SIGNAL(clicked()),
            this, SLOT(on_rmSimutransPushButton_clicked()));
  }

void RemoveSimutransPage::on_rmSimutransPushButton_clicked() {
    QMessageBox msgBox2;
    QProcess *rmSimutrans = new QProcess;

    msgBox2.setText(tr("<b>Wollen Sie die ausgewählte"
                           " Installation wirklich löschen?</b>"));
    msgBox2.setInformativeText(tr("Die Daten werden "
                                      "unwiederbringlich gelöscht!"));
    msgBox2.setIcon(QMessageBox::Question);
    msgBox2.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox2.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox2.exec();
    switch (ret) {
      case QMessageBox::Yes:
        installFortschritt->setValue(50);
        rmSimutrans->execute("rm -vfr " + gamePathremove);
        if (rmSimutrans->exitStatus() == false) {
            installFortschritt->setValue(100);
            QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                                   APP_NAME + ".conf", QSettings::NativeFormat);
            simurun_conf.beginGroup("Simutrans_Starter");
            simurun_conf.setValue("GamePath", "");
            simurun_conf.endGroup();
          }

        break;
      case QMessageBox::No:
        msgBox2.close();
        break;
      default:
        // should never be reached
        break;
      }
  }

int RemoveSimutransPage::nextId() const {
    return -1; //SetupGame::Page_Finish;
  }

FinishPage::FinishPage(QWidget *parent) : QWizardPage(parent) {
    setTitle(tr("Setup-Assistent"));
    setSubTitle(tr("Setup abschließen"));

    gameVersionLabel = new QLabel;
    gamePathLabel = new QLabel;
    pakSetsLabel = new QLabel;
    pakSetsLabel->setWordWrap(true);
    pakSetsLabel->setAlignment(Qt::AlignTop);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(9);
    formLayout->setVerticalSpacing(9);
    formLayout->setHorizontalSpacing(9);
    formLayout->addRow(tr("Installierte Simutrans Version: "), gameVersionLabel);
    formLayout->addRow(tr("Installationspfad: "), gamePathLabel);
    formLayout->addRow(tr("Installierte Paksets: "), pakSetsLabel);
    setLayout(formLayout);
  }

void FinishPage::initializePage() {
    QString pakset, pfad, version, gamePath;

    if (wizard()->hasVisitedPage(SetupGame::Page_Include))
      {
        // Lese Simutrans Version ein.
        // Read the simutrans version.
        QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
        fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inVersion(&fileVersion);
        QString simuVersion = inVersion.readLine();
        version = "<b>" + simuVersion + "</b>";

        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans_Starter");
        pfad = "<b>" + simurun_conf.value("GamePath").toString() + "</b>";
        gamePath = simurun_conf.value("GamePath").toString();
        simurun_conf.endGroup();

        // Lese Paksets in ein.
        QDir dir(gamePath + "/", "*pak*");
        QStringList filter;
        filter << "*pak*";
        QStringList dirNames = dir.entryList();

        for (int i = 0; i < (int)dirNames.size(); ++i) {
            pakset += "<b>" + dirNames[i] + "</b><br>";
          }
      }

    if (wizard()->hasVisitedPage(SetupGame::Page_Install)) {
        wizard()->setOption(QWizard::HaveCustomButton3, false);

        // Lese Simutrans Version ein.
        // Read the simutrans version.
        QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
        fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inVersion(&fileVersion);
        QString simuVersion = inVersion.readLine();
        version = "<b>" + simuVersion + "</b>";

        // Zeige Installationspfad an.
        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans_Starter");
        pfad = "<b>" + simurun_conf.value("GamePath").toString() + "</b>";
        gamePath = simurun_conf.value("GamePath").toString();
        simurun_conf.endGroup();

        // Lese Paksets in ein.
        QDir dir(gamePath + "/", "*pak*");
        QStringList filter;
        filter << "*pak*";
        QStringList dirNames = dir.entryList();

        for (int i = 0; i < (int)dirNames.size(); ++i) {
            pakset += "<b>" + dirNames[i] + "</b><br>";
          }
      }

    if (wizard()->hasVisitedPage(SetupGame::Page_InstallPakset)) {
        wizard()->setOption(QWizard::HaveCustomButton1, false);

        // Lese Simutrans Version ein.
        // Read the simutrans version.
        QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
        fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inVersion(&fileVersion);
        QString simuVersion = inVersion.readLine();
        version = "<b>" + simuVersion + "</b>";

        // Zeige Installationspfad an.
        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans_Starter");
        pfad = "<b>" + simurun_conf.value("GamePath").toString() + "</b>";
        gamePath = simurun_conf.value("GamePath").toString();
        simurun_conf.endGroup();

        // Lese Paksets in ein.
        QDir dir(gamePath + "/", "*pak*");
        QStringList filter;
        filter << "*pak*";
        QStringList dirNames = dir.entryList();

        for (int i = 0; i < (int)dirNames.size(); ++i) {
            pakset += "<b>" + dirNames[i] + "</b><br>";
          }
      }

    if (wizard()->hasVisitedPage(SetupGame::Page_UpdateGame)) {
        wizard()->setOption(QWizard::HaveCustomButton1, false);

        // Lese Simutrans Version ein.
        // Read the simutrans version.
        QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
        fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inVersion(&fileVersion);
        QString simuVersion = inVersion.readLine();
        version = "<b>" + simuVersion + "</b>";

        // Zeige Installationspfad an.
        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans_Starter");
        pfad = "<b>" + simurun_conf.value("GamePath").toString() + "</b>";
        gamePath = simurun_conf.value("GamePath").toString();
        simurun_conf.endGroup();

        // Lese Paksets in ein.
        QDir dir(gamePath + "/", "*pak*");
        QStringList filter;
        filter << "*pak*";
        QStringList dirNames = dir.entryList();

        for (int i = 0; i < (int)dirNames.size(); ++i) {
            pakset += "<b>" + dirNames[i] + "</b><br>";
          }
      }

    gameVersionLabel->setText(version);
    gamePathLabel->setText(pfad);
    pakSetsLabel->setText(pakset);
  }

int FinishPage::nextId() const {
    return -1;
  }
