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

#include "simurun.h"

simurun2::simurun2(QWidget *parent) : QMainWindow(parent) {

    setupUi(this);
    retranslateUi(this);
    setFixedSize(830, 340);

    GlobFunction ReadPakSets;
    ReadPakSets.read_simutrans_version();
    // Lese Simutrans Version ein.
    // Read the simutrans version.
    QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
    fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inVersion(&fileVersion);
    QString simuVersion = inVersion.readLine();

    setWindowTitle(tr("%1 %2 - %3")
                   .arg(APP_TITLE)
                   .arg(APP_VERSION)
                   .arg("[ Simutrans " + simuVersion + " ]"));

//    ReadPakSets.read_simutrans_version();

    createSlots();
    readSettings();
    loadPaksets();
    //    setIcons();

    setWindowIcon(QPixmap(QString(":/images/images/simurun2.png")));

    /*
     * Disabled until I write an new Handbook.
     */
    pushButtonHelp->setDisabled(true);
    actionHandbuch->setDisabled(true);

    loadSavedGames();

}

// Simutrans mit den übergebenen Argumenten starten.
// Function to run simutrans with given arguments.
void simurun2::run_simutrans(QString arguments) {
    QString start_simutrans;
    QProcess *myProcess = new QProcess;

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    start_simutrans = simurun_conf.value("GamePath").toString() +
            "/simutrans " + arguments/* + screenSize*/;
    simurun_conf.endGroup();

    myProcess->execute(start_simutrans);

}

void simurun2::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

simurun2::~simurun2() {}

void simurun2::loadGame() {
    QString argumente, resolution, server, soundAN, musikAN, loadgame, pakSet;
    pakSet = comboBoxPakSets->currentText();

    if (checkBoxFullscreen->isChecked() == true)
    {
        resolution = "-fullscreen";
    }
    // Run as game server
    if (cbServer->isChecked() == true)
    {
        server = " -server";
    }
    if (soundCheckBox->isChecked() == false)
    {
        soundAN = " -nosound";
    }
    if (musikCheckBox->isChecked() == false)
    {
        musikAN = " -nomidi";
    }
    if (rbLokalesSpiel->isChecked() == true)
    {
        loadgame = " -load " + tableWidget_SavedGames->currentItem()->text();
    }
    if (rbNetzSpiel->isChecked() == true)
    {
        loadgame = " -load net:" + tableWidget_SavedGames->currentItem()->text();
    }

    argumente =/* screenSize + resolution +*/ server + soundAN + musikAN + loadgame +
            " -objects " + pakSet +"/" +"/";

    this->hide();
    run_simutrans(argumente);
    loadSavedGames();
    this->show();
}

void simurun2::runGameAuto()
{
    QString argumente, resolution, soundAN, musikAN, loadgame, pakSet;
    pakSet = comboBoxPakSets->currentText();

    QDir dir(QDir::homePath() + "/simutrans/save/");
    dir.setSorting(QDir::Time);  // Nach Datum und Zeit sortieren.
    QStringList filter;
    filter << "*.sve"; // Dateifilter setzen.
    /*
   Gespeicherte Spielstände gefiltert und sortiert
   in eine Stringliste einlesen.
*/
    QStringList fileNames = dir.entryList(filter);

    if (checkBoxFullscreen->isChecked() == true)
    {
        resolution = "-fullscreen";
    }
    if (soundCheckBox->isChecked() == false)
    {
        soundAN = " -nosound";
    }
    if (musikCheckBox->isChecked() == false)
    {
        musikAN = " -nomidi";
    }

    loadgame = " -load " + fileNames.first(); // Den ersten Eintrag in der Liste
    // auslesen und an einen String
    // übergeben.


    /* Alle Werte in einem String zusammenführen */
    argumente = resolution +  soundAN + musikAN + loadgame +
            " -objects " + pakSet +"/";
    this->hide();
    run_simutrans(argumente);
    loadSavedGames();  // Die Liste der gespeicherten Spiele nach dem
    // beenden aktualisieren.
    this->show();
}

void simurun2::on_lastSavedGameCheckBox_clicked(bool checked)
{
    if (checked == true)
    {
        QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                               APP_NAME + ".conf", QSettings::NativeFormat);
        simurun_conf.beginGroup("Simutrans");
        simurun_conf.setValue("Run_Last_Game",
                              tableWidget_SavedGames->currentItem()->text());
        simurun_conf.endGroup();
    }

    writeSettings();
}

void simurun2::on_actionScreenshots_triggered() {
    ImageView *imageShow = new ImageView(this);
    imageShow->showMaximized();
    imageShow->show();

}

void simurun2::writeSettings() {

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    simurun_conf.setValue("Starte_Als_Server",
                          cbServer->isChecked());
    simurun_conf.setValue("Lokales_Spiel",
                          rbLokalesSpiel->isChecked());
    simurun_conf.setValue("Netzwerk_Spiel",
                          rbNetzSpiel->isChecked());
    simurun_conf.endGroup();

    simurun_conf.beginGroup("Orientation");
    simurun_conf.setValue("pos", pos());
    //  simurun_conf.setValue("size", size());
    simurun_conf.endGroup();

    simurun_conf.beginGroup("Simutrans");
    simurun_conf.setValue("pak_file_path", comboBoxPakSets->currentText());
    simurun_conf.setValue("pak_file_path_id",
                          comboBoxPakSets->currentIndex());
    simurun_conf.setValue("Fullscreen", checkBoxFullscreen->isChecked());
    simurun_conf.setValue("Sound", soundCheckBox->isChecked());
    simurun_conf.setValue("Musik", musikCheckBox->isChecked());
    simurun_conf.setValue("Last_Game_CheckBox",
                          lastSavedGameCheckBox->isChecked());
    simurun_conf.setValue("Starte_Als_Server",
                          cbServer->isChecked());
    simurun_conf.endGroup();
}

void simurun2::readSettings() {
    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gamePath = simurun_conf.value("GamePath").toString();
    cbServer->setChecked(
                simurun_conf.value("Starte_Als_Server", false).toBool());
    rbLokalesSpiel->setChecked(
                simurun_conf.value("Lokales_Spiel", true).toBool());
    rbNetzSpiel->setChecked(
                simurun_conf.value("Netzwerk_Spiel", false).toBool());
    parseGameXml = simurun_conf.value("Parse_XML", true).toBool();
    readGameFiles = simurun_conf.value("Read_Dir", false).toBool();
    simurun_conf.endGroup();

    simurun_conf.beginGroup("Orientation");
    QPoint pos = simurun_conf.value("pos", QPoint(250, 215)).toPoint();
    //  QSize size = simurun_conf.value("size", QSize(565, 320)).toSize();
    simurun_conf.endGroup();

    simurun_conf.beginGroup("Simutrans");
    comboBoxPakSets->setCurrentIndex(comboBoxPakSets->
                                     findText(simurun_conf.value(
                                                  "pak_file_path").toString(),
                                              Qt::MatchExactly));
    checkBoxFullscreen->setChecked(
                simurun_conf.value("Fullscreen", false).toBool());
    soundCheckBox->setChecked(
                simurun_conf.value("Sound", false).toBool());
    musikCheckBox->setChecked(
                simurun_conf.value("Musik", true).toBool());
    lastSavedGameCheckBox->setChecked(
                simurun_conf.value("Last_Game_CheckBox", false).toBool());
    gstartArguments = simurun_conf.value("Run_Last_Game").toString();
    screenSize = " -screensize " + simurun_conf.value("Screensize").toString();
    simurun_conf.endGroup();

    move(pos);
    //  resize(size);

    loadGameType();
}

void simurun2::on_comboBoxPakSets_activated(QString) {
    writeSettings();
}

void simurun2::on_checkBoxFullscreen_clicked() {
    writeSettings();
}

void simurun2::loadPaksets() {

    comboBoxPakSets->clear();
    // Lese Paksets in die Combobox ein.
    // Fill ComboBox with the paksets
    QDir dir(gamePath + "/", "pak*");
    QStringList filter;
    filter << "pak*";

    QStringList dirNames = dir.entryList();

    for (int i = 0; i < (int)dirNames.size(); ++i) {
        comboBoxPakSets->addItem(QPixmap(":/images/images/pak-file.png"), dirNames[i]);
    }
    readSettings();
}

void simurun2::loadSavedGames() {

    if (rbLokalesSpiel->isChecked() == true)
    {
        tableWidget_SavedGames->setRowCount(0);

        QFile xmlFile(gamePath + CACHED_XML);
        xmlFile.open(QIODevice::ReadOnly | QIODevice::Text);

        if (readGameFiles == true){
            /*
             * DIENT WEITER ALS NOTFALL MODUS!
             *
             * Für ältere Simutrans Versionen.
             * Falls keine _cache.xml existiert, die gespeicherten Spielstände
             * im Verzeichnis auslesen.
             *
             * THIS IS STILL USED AS FALLBACK MODE!
             *
             * For older Simutrans versions.
             * If _cache.xml not exist read the saved games in the folder.
             */
            QDir gamesDir(gamePath + "/save/", "*.sve");
            gamesDir.setFilter(QDir::Files | QDir::NoSymLinks);
            gamesDir.setSorting(QDir::Time);

            QFileInfoList list = gamesDir.entryInfoList();

            // Kopfzeile für die Tabellenansicht.
            // Header for the table widget.
            QStringList header;
            header << tr("Spielstand")
                    << tr("Gespeichert");

            for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);

                QTableWidgetItem *gamesItem = new QTableWidgetItem;
                gamesItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                gamesItem->setData(0, fileInfo.fileName());

                QTableWidgetItem *dateItem = new QTableWidgetItem;
                dateItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                dateItem->setData(0, fileInfo.lastModified().toString(Qt::SystemLocaleDate));

                tableWidget_SavedGames->setColumnCount(2);
                tableWidget_SavedGames->setHorizontalHeaderLabels(header);
                tableWidget_SavedGames->insertRow(i);
//                tableWidget_SavedGames->setColumnHidden(1, true);
                tableWidget_SavedGames->setItem(i, 0, gamesItem);
                tableWidget_SavedGames->setItem(i, 1, dateItem);
                tableWidget_SavedGames->resizeColumnsToContents();
            }

        } else if (parseGameXml == true) {
            /*
             * Die Spielstände aus der Datei _cache.xml parsen.
             * Parse the saved games from file _cache.xml.
             */
            Parse_XML_Local readXML;
            readXML.readFile(tableWidget_SavedGames, &xmlFile);

        }
    } else if(rbNetzSpiel->isChecked() == true)	{

        tableWidget_SavedGames->setRowCount(0);

        // Kopfzeile für die Tabellenansicht.
        // Header for the table widget.
        QStringList header;
        header << tr("Spiel")
               << tr("Pak-Set")
               << tr("Gespeichert")
               << tr("Größe");

        tableWidget_SavedGames->setColumnCount(4);
        tableWidget_SavedGames->setHorizontalHeaderLabels(header);

    }

    if (tableWidget_SavedGames->rowCount() == 0)
    {
        pushButtonLoadGame->setEnabled(false);
        pushButtonDeleteGame->setEnabled(false);
        lastSavedGameCheckBox->setEnabled(false);
        lastSavedGameCheckBox->setChecked(false);
        tableWidget_SavedGames->clear();
        tableWidget_SavedGames->setEnabled(false);
        tableWidget_SavedGames->setToolTip(
                    tr("Es gibt keine gespeicherten Spielstände!"));
    } else {
        tableWidget_SavedGames->setEnabled(true);
        pushButtonLoadGame->setEnabled(true);
        pushButtonDeleteGame->setEnabled(true);
        lastSavedGameCheckBox->setEnabled(true);
        tableWidget_SavedGames->setFocus();
    }
}

void simurun2::on_actionUpdateSimutrans_triggered() {
    SetupGame *runSetup = new SetupGame(this);
    runSetup->exec();
    loadPaksets();
}

void simurun2::on_comboBoxPakSets_currentIndexChanged(const QString)
{
    //writeSettings();
}

void simurun2::on_actionSimutransHomepage_triggered(){
    QString openWebSite = "http://www.simutrans.com/";
    QDesktopServices::openUrl(openWebSite);
}

void simurun2::on_actionSimutransWiki_triggered(){
    QString openWebSite = "http://wiki.simutrans.com/";
    QDesktopServices::openUrl(openWebSite);
}

void simurun2::on_actionSimutransForum_triggered(){
    QString openWebSite = "http://forum.simutrans.com/";
    QDesktopServices::openUrl(openWebSite);
}

void simurun2::on_actionSimutrans_triggered(){
    QString openWebSite = "http://sourceforge.net/projects/simutrans/files/";
    QDesktopServices::openUrl(openWebSite);
}

void simurun2::on_actionSS2_Update_triggered(){
    Updates *updates = new Updates;
    updates->exec();
}

void simurun2::on_soundCheckBox_clicked()
{
    writeSettings();
}

void simurun2::on_musikCheckBox_clicked()
{
    writeSettings();
}

void simurun2::on_lastSavedGameCheckBox_clicked()
{
    writeSettings();
}

void simurun2::on_rbNetzSpiel_clicked()
{
    loadSavedGames();
    loadGameType();
    writeSettings();
}


void simurun2::on_rbLokalesSpiel_clicked()
{
    loadSavedGames();
    loadGameType();
    writeSettings();
}

void simurun2::loadGameType()
{
    if (rbNetzSpiel->isChecked() == true)
    {
        tableWidget_SavedGames->clear();
        cbServer->setEnabled(false);
        tableWidget_SavedGames->setToolTip(tr("Eine gespeicherte Verbindung laden."));
        gb_Saved_Games->setTitle(tr("Gespeicherte Verbindungen"));
        pushButtonLoadGame->setText(tr("&Verbinden"));
        pushButtonDeleteGame->setEnabled(false);
        loadSavedGames();
    }

    if (rbLokalesSpiel->isChecked() == true)
    {
        tableWidget_SavedGames->clear();
        cbServer->setEnabled(true);
        tableWidget_SavedGames->setToolTip(tr("Ein gespeichertes Spiel laden."));
        gb_Saved_Games->setTitle(tr("Gespeicherte Spielstände"));
        pushButtonLoadGame->setText(tr("Sta&rte Spiel"));
        loadSavedGames();
    }

    if (tableWidget_SavedGames->rowCount() == 0)
    {
        pushButtonLoadGame->setEnabled(false);
        pushButtonDeleteGame->setEnabled(false);
        lastSavedGameCheckBox->setEnabled(false);
        lastSavedGameCheckBox->setChecked(false);
    } else {
        pushButtonLoadGame->setEnabled(true);
        pushButtonDeleteGame->setEnabled(true);
        lastSavedGameCheckBox->setEnabled(true);
        tableWidget_SavedGames->setFocus();

        if (lastSavedGameCheckBox->isChecked() == true)
        {
            // Wenn die Option "Letztes Spiel starten" aktiviert ist,
            // wird das zuletzt gespeicherte Spiel gestartet.
            runGameAuto();
        }
    }
    //  writeSettings();
}

void simurun2::spielLadenRowChanged()
{
    pushButtonLoadGame->setEnabled(true);
    pushButtonDeleteGame->setEnabled(true);

    if(rbNetzSpiel->isChecked() == true)
    {
        pushButtonDeleteGame->setEnabled(false);
    }
}

void simurun2::createSlots()
{
    connect(tableWidget_SavedGames, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(loadGame()));
    connect(tableWidget_SavedGames, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
            this, SLOT(spielLadenRowChanged()));

    connect(action_ueber_Qt, SIGNAL(triggered()),
            this, SLOT(ueberQt()));
    connect(action_ueber, SIGNAL(triggered()),
            this, SLOT(ueberSimurun()));
    connect(actionHandbuch, SIGNAL(triggered()),
            this, SLOT(helpBrowser()));
    connect(actionBeenden, SIGNAL(triggered()),
            this, SLOT(simurunExit()));

    connect(pushButtonLoadGame, SIGNAL(clicked()),
            this, SLOT(loadGame()));
    connect(pushButtonAbout, SIGNAL(clicked()),
            this, SLOT(ueberSimurun()));
    connect(pushButtonHelp, SIGNAL(clicked()),
            this, SLOT(helpBrowser()));
    connect(pushButtonNewGame, SIGNAL(clicked()),
            this, SLOT(newGame()));
    connect(pushButtonExit, SIGNAL(clicked()),
            this, SLOT(simurunExit()));
    connect(pushButtonDeleteGame, SIGNAL(clicked()),
            this, SLOT(deleteGame()));
    connect(pushButtonSettings, SIGNAL(clicked()),
            this, SLOT(loadSettings()));
    connect(pushButtonReload, SIGNAL(clicked()),
            this, SLOT(loadSavedGames()));
}

void simurun2::ueberSimurun()
{
    About_SS2 *ueber = new About_SS2(this);
    ueber->exec();
}

void simurun2::ueberQt()
{
    QMessageBox msgb;
    msgb.aboutQt(this);
}

void simurun2::helpBrowser()
{
    Help_Browser *myHelp = new Help_Browser(this);
    myHelp->show();
}

void simurun2::simurunExit() {
    writeSettings();
    close();
}
void simurun2::newGame()
{
    QString arguments, resolution, server, soundAN, musikAN, pakSet;

    pakSet = comboBoxPakSets->currentText();

    // Run as game server
    if (cbServer->isChecked() == true)
    {
        server = " -server";
    }
    if (checkBoxFullscreen->isChecked() == true)
    {
        resolution = "-fullscreen";
    }
    if (soundCheckBox->isChecked() == false)
    {
        soundAN = " -nosound";
    }
    if (musikCheckBox->isChecked() == false)
    {
        musikAN = " -nomidi";
    }

    arguments = resolution + server + soundAN + musikAN +
            " -objects " + pakSet +"/";

    this->hide();
    run_simutrans(arguments);
    loadSavedGames();
    this->show();
}

void simurun2::loadSettings()
{
    ConfigureClass *SettGui = new ConfigureClass;
//    SettGui->showMaximized();
    SettGui->show();
}

void simurun2::deleteGame()
{
    QFile rmFile;
    QMessageBox msgBox;

    msgBox.setText(tr("<b>Wollen Sie den gespeichertern"
                          " Spielstand wirklich löschen?</b>"));
    msgBox.setInformativeText("<b>" + tableWidget_SavedGames->item(tableWidget_SavedGames->currentRow(), 0)->data(0).toString() +
                              tr("</b> wird unwiederbringlich gelöscht!"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        rmFile.remove(QDir::homePath() + "/simutrans/save/" +
                      tableWidget_SavedGames->item(tableWidget_SavedGames->currentRow(), 0)->data(0).toString());
        tableWidget_SavedGames->removeRow(tableWidget_SavedGames->currentRow());
        if (tableWidget_SavedGames->rowCount() == 0)
        {
            pushButtonLoadGame->setEnabled(false);
            pushButtonDeleteGame->setEnabled(false);
            lastSavedGameCheckBox->setEnabled(false);
            lastSavedGameCheckBox->setChecked(false);
            tableWidget_SavedGames->clear();
            tableWidget_SavedGames->setEnabled(false);
            tableWidget_SavedGames->setToolTip(
                        tr("Es gibt keine gespeicherten Spielstände!"));
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
