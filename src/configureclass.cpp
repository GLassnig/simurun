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

#include "configureclass.h"

ConfigureClass::ConfigureClass(QWidget *parent) : QWidget(parent)
{
    languages = new LanguageTools();
    fileToNiceName = new QMap<QString, QString>();

    setupUi(this);
    setWindowTitle(tr("%1 %2 - %3 %4")
                   .arg(APP_TITLE)
                   .arg(APP_VERSION)
                   .arg("Konfiguration")
                   .arg("[ " + tabMain->tabText(
                            tabMain->currentIndex()) + " ]"));

    tabMain->setCurrentIndex (0);
    tabSettings->setCurrentIndex (0);
    editServerFrame->setVisible(false);
    pbAendern->setVisible(false);

    QStringList headerLabels;
    headerLabels << tr("Serverlink") << tr("Pakset") << tr("Kurzbeschreibung");
    serverTableWidget->setColumnCount(3);
    serverTableWidget->setHorizontalHeaderLabels(headerLabels);

    createLanguageMenu();
    //	readSettings();
    loadValues();
    readSettings();
    readSimutransSettings();


    //    setIcons();
    readServerXml();

    label_166->setHidden(true);
    pushButton_ChDir_Simutrans->setEnabled(false);
}

void ConfigureClass::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void ConfigureClass::readSettings()
{
    QString locale = QLocale::system().name();
    QString defaultLanguage = "simurun2._" + locale + ".qm";

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    cbStaedtenamen->setCurrentIndex(
                simurun_conf.value("city_lang_id", 0).toInt());
    cbIconDesktop->setChecked(
                simurun_conf.value("desktop_icon", false).toBool());
    LanguageAppGroup->setChecked(
                simurun_conf.value("Set_Language",false).toBool());
    cbSprachen->setCurrentIndex(cbSprachen->findText(
                                    simurun_conf.value("languageNiceName",defaultLanguage).toString(),
                                    Qt::MatchExactly));
    gamePath = simurun_conf.value("GamePath").toString();
    txtPfad->setText(simurun_conf.value("GamePath").toString());
    radioButton_ParseXml->setChecked(simurun_conf.value("Parse_XML", true).toBool());
    radioButton_ReadDir->setChecked(simurun_conf.value("Read_Dir", false).toBool());
    simurun_conf.endGroup();

    txtAktivesPakset->setText(simurun_conf.value(
                                  "Simutrans/pak_file_path").toString());
    cbPakSets->setCurrentIndex(cbPakSets->
                               findText(simurun_conf.value(
                                            "Simutrans/pak_file_path").toString(),
                                        Qt::MatchExactly));
}

void ConfigureClass::writeSettings()
{
    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    simurun_conf.setValue("city_lang_id", cbStaedtenamen->currentIndex());
    simurun_conf.setValue("Set_Language", LanguageAppGroup->isChecked());
    simurun_conf.setValue("languageNiceName",cbSprachen->currentText());
    simurun_conf.setValue("languageFileName",
                          fileToNiceName->value(cbSprachen->currentText()));
    simurun_conf.setValue("desktop_icon", cbIconDesktop->isChecked());
    simurun_conf.setValue("Parse_XML", radioButton_ParseXml->isChecked());
    simurun_conf.setValue("Read_Dir", radioButton_ReadDir->isChecked());
    simurun_conf.endGroup();


}

void ConfigureClass::readSimutransSettings()
{

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           cbPakSets->currentText() + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup(cbPakSets->currentText());
    // PROGRAM STUFF
    cbSingleUser->setCurrentIndex(
                simurun_conf.value("singleuser_install", 0).toInt());
    sbRoutesteps->setValue(
                simurun_conf.value("max_route_steps", 100000).toInt());
    cbBeginner->setCurrentIndex(
                simurun_conf.value("first_beginner", 0).toInt());
    spPreisfaktor->setValue(
                simurun_conf.value("beginner_price_factor", 1500).toInt());
    spTransfers->setValue(
                simurun_conf.value("max_transfers", 9).toInt());
    sbSpruenge->setValue(
                simurun_conf.value("max_hops", 2000).toInt());
    sbAbdeckungen->setValue(
                simurun_conf.value("station_coverage", 2).toInt());
    sbAbstRath->setValue(
                simurun_conf.value("minimum_city_distance", 16).toInt());
    spinBoxAttractionDistance->setValue(
                simurun_conf.value("special_building_distance", 3).toInt());

    //DISPLAY STUFF
    cbPassanten->setCurrentIndex(
                simurun_conf.value("random_pedestrians", 1).toInt());
    cbInfo->setCurrentIndex(
                simurun_conf.value("only_single_info", 1).toInt());
    cbInfoAutos->setCurrentIndex(
                simurun_conf.value("pedes_and_car_info", 1).toInt());
    cbNummeriert->setCurrentIndex(
                simurun_conf.value("numbered_stations", 0).toInt());
    cbPassagiereStationen->setCurrentIndex(
                simurun_conf.value("stop_pedestrians", 1).toInt());
    cbStationsnamen->setCurrentIndex(
                simurun_conf.value("show_names", 3).toInt());
    cbBaumInfo->setCurrentIndex(
                simurun_conf.value("tree_info", 1).toInt());
    cbRathausInfo->setCurrentIndex(
                simurun_conf.value("townhall_info", 1).toInt());
    cbBodenInfo->setCurrentIndex(
                simurun_conf.value("ground_info", 1).toInt());
    cbLinksfahren->setCurrentIndex(
                simurun_conf.value("drive_left", 0).toInt());
    sbKfzLebensdauer->setValue(
                simurun_conf.value("default_citycar_life", 36).toInt());
    sbWasser->setValue(
                simurun_conf.value("water_animation_ms", 250).toInt());
    sbTiere->setValue(
                simurun_conf.value("random_wildlife_probability", 1000).toInt());
    sbLandschaft->setValue(
                simurun_conf.value("random_grounds_probability", 10).toInt());
    sbKfzLevel->setValue(
                simurun_conf.value("citycar_level", 5).toInt());
    cB_Vehicle_Status->setCurrentIndex(
                simurun_conf.value("show_vehicle_states", 1).toInt());
    cB_Visualize_Schedule->setCurrentIndex(
                simurun_conf.value("visualize_schedule", 0).toInt());
    sB_Height_Z_Direction->setValue(
                simurun_conf.value("tile_height", 8).toInt());
    spinBox_height_conversion_factor->setValue(
                simurun_conf.value("height_conversion_factor", 4).toInt());
    comboBoxDrawEarthBorder->setCurrentIndex(
                simurun_conf.value("draw_earth_border", 1).toInt());
    comboBoxDrawOutsideTile->setCurrentIndex(
                simurun_conf.value("draw_outside_tile", 0).toInt());
    spinBoxBackgroundColor->setValue(
                simurun_conf.value("background_color", 210).toInt());
    comboBoxDeleteGame->setCurrentIndex(
                simurun_conf.value("show_delete_buttons", 0).toInt());
    comboBoxInvertedSchedule->setCurrentIndex(
                simurun_conf.value("hide_rail_return_ticket", 1).toInt());
    spinBoxRemoveDeadPlayer->setValue(
                simurun_conf.value("remove_dummy_player_months", 6).toInt());
    spinBoxAbandonedPlayer->setValue(
                simurun_conf.value("spinBoxAbandonedPlayer", 0).toInt());

    //WINDOW STUFF
    cbButtons->setCurrentIndex(
                simurun_conf.value("window_buttons_right", 0).toInt());
    cbRahmen->setCurrentIndex(
                simurun_conf.value("window_frame_active", 0).toInt());
    cbToolTipps->setCurrentIndex(
                simurun_conf.value("show_tooltips", 1).toInt());
    sbTooltippsDelay->setValue(
                simurun_conf.value("tooltip_delay", 500).toInt());
    sbToolTippsDauer->setValue(
                simurun_conf.value("tooltip_duration", 1500).toInt());

    // TIME STUFF
    sbJahr->setValue(
                simurun_conf.value("starting_year", 1930).toInt());
    sbMonate->setValue(
                simurun_conf.value("starting_month", 1).toInt());
    sbForwardSpeed->setValue(
                simurun_conf.value("bits_per_month", 20).toInt());
    cbNeueKfz->setCurrentIndex(
                simurun_conf.value("use_timeline", 3).toInt());
    cbDatumsFormat->setCurrentIndex(
                simurun_conf.value("show_month", 1).toInt());

    // SYSTEM STUFF
    cbSpeicherFormat->setCurrentIndex(
                simurun_conf.value("saveformat", 0).toInt());
    cbMonatIntervall->setCurrentIndex(
                simurun_conf.value("autosave", 0).toInt());
    cbMessages->setCurrentIndex(
                simurun_conf.value("add_player_name_to_message", 1).toInt());
    sbFPS->setValue(
                simurun_conf.value("frames_per_second", 25).toInt());
    sbSimpleDawTiles->setValue(
                simurun_conf.value("simple_drawing_tile_size", 24).toInt());
    sbFastForward->setValue(
                simurun_conf.value("fast_forward", 100).toInt());
    spinBoxThreadsToUse->setValue(
                simurun_conf.value("threads", 4).toInt());
    comboBoxDrawingFast->setCurrentIndex(
                simurun_conf.value("simple_drawing_fast_forward", 1).toInt());

    // NETWORK STUFF
    sbDelay->setValue(
                simurun_conf.value("server_frames_ahead", 4).toInt());
    sbUpdates->setValue(
                simurun_conf.value("server_frames_per_step", 4).toInt());
    sbTimeDelay->setValue(
                simurun_conf.value("additional_client_frames_behind", 0).toInt());
    sbClientCheck->setValue(
                simurun_conf.value("server_frames_between_checks", 256).toInt());
    cbServerAnnounce->setCurrentIndex(
                simurun_conf.value("server_announce", 0).toInt());
    sbAboIntervall->setValue(
                simurun_conf.value("server_announce_intervall", 900).toInt());
    txtServerName->setText(
                simurun_conf.value("server_name").toString());
    txtKommentar->setPlainText(
                simurun_conf.value("server_comment").toString());
    txtAdminPW->setText(
                simurun_conf.value("server_admin_pw").toString());
    cbServerPause->setCurrentIndex(
                simurun_conf.value("pause_server_no_clients", 1).toInt());
    txtServerAdresse->setText(
                simurun_conf.value("server_dns").toString());
    txtServerEmail->setText(
                simurun_conf.value("server_email").toString());
    txtPakSetUrl->setText(
                simurun_conf.value("server_pakurl").toString());
    txtServerInfo->setText(
                simurun_conf.value("server_infurl").toString());
    txtNickName->setText(
                simurun_conf.value("Nickname").toString());
    spinBoxChatTransparency->setValue(
                simurun_conf.value("chat_transparency", 75).toInt());

    // FACTORY STUFF
    sbProzent->setValue(
                simurun_conf.value("crossconnect_factories_percentage", 33).toInt());
    sbErweitern->setValue(
                simurun_conf.value("industry_increase_every", 2000).toInt());
    cbBeliefern->setCurrentIndex(
                simurun_conf.value("just_in_time", 1).toInt());
    spinBoxMinAmount->setValue(
                simurun_conf.value("maximum_intransit_percentage", 0).toInt());
    sbPromille->setValue(
                simurun_conf.value("electric_promille", 330).toInt());
    cbFabrikAnforderung->setCurrentIndex(
                simurun_conf.value("factory_enforce_demand", 1).toInt());
    spinBoxMinFactorySpacing->setValue(
                simurun_conf.value("factory_spacing", 6).toInt());
    spinBoxMaxFactorySpacing->setValue(
                simurun_conf.value("max_factory_spacing", 40).toInt());
    spinBoxMaxFactorySpacingPercent->setValue(
                simurun_conf.value("max_factory_spacing_percentage", 25).toInt());

    // GOODS/PASSENGER STUFF
    sbPassInterwal->setValue(
                simurun_conf.value("factory_arrival_periods", 4).toInt());
    sbMultiplikatorPassagiere->setValue(
                simurun_conf.value("passenger_multiplier", 40).toInt());
    sbMultiplikatorEnergie->setValue(
                simurun_conf.value("electricity_multiplier", 0).toInt());
    sbMultiplikatorPost->setValue(
                simurun_conf.value("mail_multiplier", 20).toInt());
    sbMultiplikatorGoods->setValue(
                simurun_conf.value("goods_multiplier", 20).toInt());
    sbPassagierFoktor->setValue(
                simurun_conf.value("passenger_factor", 10).toInt());
    cbStationUeberfuellt->setCurrentIndex(
                simurun_conf.value("no_routing_over_overcrowded", 0).toInt());
    sbArbeiteraufkommen->setValue(
                simurun_conf.value("factory_worker_percentage", 33).toInt());
    sbTouristenaufkommen->setValue(
                simurun_conf.value("tourist_percentage", 16).toInt());
    sbRadius->setValue(
                simurun_conf.value("factory_worker_radius", 77).toInt());
    sbMinCities->setValue(
                simurun_conf.value("factory_worker_minimum_towns", 2).toInt());
    cbZielUeberfuellt->setCurrentIndex(
                simurun_conf.value("avoid_overcrowding", 0).toInt());
    cbTransportCash->setCurrentIndex(
                simurun_conf.value("pay_for_total_distance", 0).toInt());
    sbMaxCities->setValue(
                simurun_conf.value("factory_worker_maximum_towns", 4).toInt());
    sbUnter_1000->setValue(
                simurun_conf.value("growthfactor_villages", 400).toInt());
    sb1000_10000->setValue(
                simurun_conf.value("growthfactor_cities", 200).toInt());
    sbUeber_10000->setValue(
                simurun_conf.value("growthfactor_capitals", 100).toInt());
    cbKapazitaeten->setCurrentIndex(
                simurun_conf.value("seperate_halt_capacities", 0).toInt());
    sbArbeiterKleinerRadius->setValue(
                simurun_conf.value("city_short_range_percentage", 60).toInt());
    ArbeiterGrosserRadius->setValue(
                simurun_conf.value("city_medium_range_percentage", 30).toInt());
    sbStadtRadiusKurz->setValue(
                simurun_conf.value("city_short_range_radius", 50).toInt());
    sbStadtRadiusMittel->setValue(
                simurun_conf.value("city_medium_range_radius", 100).toInt());
    sb_Jahr_1->setValue(
                simurun_conf.value("sb_Jahr_1", 1835).toInt());
    sb_Jahr_Ziel_1->setValue(
                simurun_conf.value("sb_Jahr_Ziel_1", 1).toInt());
    sb_Jahr_2->setValue(
                simurun_conf.value("sb_Jahr_2", 1835).toInt());
    sb_Jahr_Ziel_2->setValue(
                simurun_conf.value("sb_Jahr_Ziel_2", 1).toInt());
    sb_Jahr_3->setValue(
                simurun_conf.value("sb_Jahr_3", 1835).toInt());
    sb_Jahr_Ziel_3->setValue(
                simurun_conf.value("sb_Jahr_Ziel_3", 1).toInt());
    sb_Jahr_4->setValue(
                simurun_conf.value("sb_Jahr_4", 1835).toInt());
    sb_Jahr_Ziel_4->setValue(
                simurun_conf.value("sb_Jahr_Ziel_4", 1).toInt());
    sb_Jahr_5->setValue(
                simurun_conf.value("sb_Jahr_5", 1835).toInt());
    sb_Jahr_Ziel_5->setValue(
                simurun_conf.value("sb_Jahr_Ziel_5", 1).toInt());
    sb_Jahr_6->setValue(
                simurun_conf.value("sb_Jahr_6", 1835).toInt());
    sb_Jahr_Ziel_6->setValue(
                simurun_conf.value("sb_Jahr_Ziel_6", 1).toInt());
    sb_Jahr_7->setValue(
                simurun_conf.value("sb_Jahr_7", 1835).toInt());
    sb_Jahr_Ziel_7->setValue(
                simurun_conf.value("sb_Jahr_Ziel_7", 1).toInt());
    sb_Jahr_8->setValue(
                simurun_conf.value("sb_Jahr_8", 1835).toInt());
    sb_Jahr_Ziel_8->setValue(
                simurun_conf.value("sb_Jahr_Ziel_8", 1).toInt());
    sb_Jahr_9->setValue(
                simurun_conf.value("sb_Jahr_9", 1835).toInt());
    sb_Jahr_Ziel_9->setValue(
                simurun_conf.value("sb_Jahr_Ziel_9", 1).toInt());
    sb_Jahr_10->setValue(
                simurun_conf.value("sb_Jahr_10", 1835).toInt());
    sb_Jahr_Ziel_10->setValue(
                simurun_conf.value("sb_Jahr_Ziel_10", 1).toInt());

    // MONEY STUFF
    cbStatistik->setCurrentIndex(
                simurun_conf.value("left_to_right_graphs", 0).toInt());
    cbAusgemustert->setCurrentIndex(
                simurun_conf.value("allow_buying_obsolete_vehicles", 1).toInt());
    sbWertverlustKfz->setValue(
                simurun_conf.value("used_vehicle_reduction", 0).toInt());
    sbStartkapital->setValue(
                simurun_conf.value("starting_money", 2000000).toInt());
    sbStartkapital_1->setValue(
                simurun_conf.value("Startkapital_1", 2000000).toInt());
    sbStartJahr_1->setValue(
                simurun_conf.value("StartJahr_1", 1850).toInt());
    cbInterpoliert_1->setCurrentIndex(
                simurun_conf.value("Interpoliert_1", 0).toInt());
    sbStartkapital_2->setValue(
                simurun_conf.value("Startkapital_2", 2000000).toInt());
    sbStartJahr_2->setValue(
                simurun_conf.value("StartJahr_2", 1850).toInt());
    cbInterpoliert_2->setCurrentIndex(
                simurun_conf.value("Interpoliert_2", 0).toInt());
    sbStartkapital_3->setValue(
                simurun_conf.value("Startkapital_3", 2000000).toInt());
    sbStartJahr_3->setValue(
                simurun_conf.value("StartJahr_3", 1850).toInt());
    cbInterpoliert_3->setCurrentIndex(
                simurun_conf.value("Interpoliert_3", 0).toInt());
    sbStartkapital_4->setValue(
                simurun_conf.value("Startkapital_4", 2000000).toInt());
    sbStartJahr_4->setValue(
                simurun_conf.value("StartJahr_4", 1850).toInt());
    cbInterpoliert_4->setCurrentIndex(
                simurun_conf.value("Interpoliert_4", 0).toInt());
    sbStartkapital_5->setValue(
                simurun_conf.value("Startkapital_5", 2000000).toInt());
    sbStartJahr_5->setValue(
                simurun_conf.value("StartJahr_5", 1850).toInt());
    cbInterpoliert_5->setCurrentIndex(
                simurun_conf.value("Interpoliert_5", 0).toInt());
    sbStartkapital_6->setValue(
                simurun_conf.value("Startkapital_6", 2000000).toInt());
    sbStartJahr_6->setValue(
                simurun_conf.value("StartJahr_6", 1850).toInt());
    cbInterpoliert_6->setCurrentIndex(
                simurun_conf.value("Interpoliert_6", 0).toInt());
    sbStartkapital_7->setValue(
                simurun_conf.value("Startkapital_7", 2000000).toInt());
    sbStartJahr_7->setValue(
                simurun_conf.value("StartJahr_7", 1850).toInt());
    cbInterpoliert_7->setCurrentIndex(
                simurun_conf.value("Interpoliert_7", 0).toInt());
    sbStartkapital_8->setValue(
                simurun_conf.value("Startkapital_8", 2000000).toInt());
    sbStartJahr_8->setValue(
                simurun_conf.value("StartJahr_8", 1850).toInt());
    cbInterpoliert_8->setCurrentIndex(
                simurun_conf.value("Interpoliert_8", 0).toInt());
    sbStartkapital_9->setValue(
                simurun_conf.value("Startkapital_9", 2000000).toInt());
    sbStartJahr_9->setValue(
                simurun_conf.value("StartJahr_9", 1850).toInt());
    cbInterpoliert_9->setCurrentIndex(
                simurun_conf.value("Interpoliert_9", 0).toInt());
    sbStartkapital_10->setValue(
                simurun_conf.value("Startkapital_10", 2000000).toInt());
    sbStartJahr_10->setValue(
                simurun_conf.value("StartJahr_10", 1850).toInt());
    cbInterpoliert_10->setCurrentIndex(
                simurun_conf.value("Interpoliert_10", 0).toInt());
    sbGebaeude->setValue(
                simurun_conf.value("maintenance_building", 2000).toInt());
    sbSchiffsdock->setValue(
                simurun_conf.value("cost_multiply_dock", 500).toInt());
    sbBahnhof->setValue(
                simurun_conf.value("cost_multiply_station", 600).toInt());
    sbHaltestelle->setValue(
                simurun_conf.value("cost_multiply_roadstop", 400).toInt());
    sbTerminal->setValue(
                simurun_conf.value("cost_multiply_airterminal", 3000).toInt());
    sbPosthalt->setValue(
                simurun_conf.value("cost_multiply_post", 300).toInt());
    sbHauptquartier->setValue(
                simurun_conf.value("cost_multiply_headquarter", 1000).toInt());
    sbHanger->setValue(
                simurun_conf.value("cost_depot_air", 5000).toInt());
    sbLockschuppen->setValue(
                simurun_conf.value("cost_depot_rail", 1000).toInt());
    sbKfzdepot->setValue(
                simurun_conf.value("cost_depot_road", 1300).toInt());
    sbSchiffsdepot->setValue(
                simurun_conf.value("cost_depot_ship", 2500).toInt());
    sbSignale->setValue(
                simurun_conf.value("cost_signal", 500).toInt());
    sbTunnel->setValue(
                simurun_conf.value("cost_tunnel", 10000).toInt());
    sbDrittesGleis->setValue(
                simurun_conf.value("cost_third_rail", 80).toInt());
    sbLanderweiterung->setValue(
                simurun_conf.value("cost_buy_land", 100).toInt());
    sbLandaenderung->setValue(
                simurun_conf.value("cost_alter_land", 1000).toInt());
    sbSlope->setValue(
                simurun_conf.value("cost_set_slope", 2500).toInt());
    sbNeueStadt->setValue(
                simurun_conf.value("cost_found_city", 5000000).toInt());
    sbIndustrie->setValue(
                simurun_conf.value("cost_multiply_found_industry", 20000).toInt());
    sbBaumFaellen->setValue(
                simurun_conf.value("cost_remove_tree", 100).toInt());
    sbAbreissen->setValue(
                simurun_conf.value("cost_multiply_remove_haus", 1000).toInt());
    sbFeldEntfernen->setValue(
                simurun_conf.value("cost_multiply_remove_field", 5000).toInt());
    sbTrafoKaufen->setValue(
                simurun_conf.value("cost_transformer", 2500).toInt());
    sbTrafoWartung->setValue(
                simurun_conf.value("cost_maintain_transformer", 20).toInt());
    sb_Maut->setValue(
                simurun_conf.value("toll_runningcost_percentage", 0).toInt());
    sb_StrKosten->setValue(
                simurun_conf.value("toll_waycost_percentage", 0).toInt());
    sb_Bonus_Basefactor->setValue(
                simurun_conf.value("bonus_basefactor", 125).toInt());

    // ROAD/RIVER/FORREST STUFF
    sbLandStrLaenge->setValue(
                simurun_conf.value("intercity_road_length", 200).toInt());
    cbStraightWays->setCurrentIndex(
                simurun_conf.value("straight_way_without_control", 0).toInt());
    // Road settings Simutrans 102.2.2 and before.
    cbStadtStrBelag->setCurrentIndex(
                simurun_conf.value("city_road_type", 2).toInt());
    cbLandStrBelag->setCurrentIndex(
                simurun_conf.value("intercity_road_type", 3).toInt());
    // Road settings since Simutrans 110.0 and higher.
    cbStadtStrBelag_1->setCurrentIndex(
                simurun_conf.value("city_road_1", 2).toInt());
    sbStadtStrStart_1->setValue(
                simurun_conf.value("sbStadtStrStart_1", 1850).toInt());
    sbStadtStrEnd_1->setValue(
                simurun_conf.value("sbStadtStrEnd_1", 2100).toInt());
    cbStadtStrBelag_2->setCurrentIndex(
                simurun_conf.value("city_road_2", 2).toInt());
    sbStadtStrStart_2->setValue(
                simurun_conf.value("sbStadtStrStart_2", 1850).toInt());
    sbStadtStrEnd_2->setValue(
                simurun_conf.value("sbStadtStrEnd_2", 2100).toInt());
    cbStadtStrBelag_3->setCurrentIndex(
                simurun_conf.value("city_road_3", 2).toInt());
    sbStadtStrStart_3->setValue(
                simurun_conf.value("sbStadtStrStart_3", 1850).toInt());
    sbStadtStrEnd_3->setValue(
                simurun_conf.value("sbStadtStrEnd_3", 2100).toInt());
    cbStadtStrBelag_4->setCurrentIndex(
                simurun_conf.value("city_road_4", 2).toInt());
    sbStadtStrStart_4->setValue(
                simurun_conf.value("sbStadtStrStart_4", 1850).toInt());
    sbStadtStrEnd_4->setValue(
                simurun_conf.value("sbStadtStrEnd_4", 2100).toInt());
    cbStadtStrBelag_5->setCurrentIndex(
                simurun_conf.value("city_road_5", 2).toInt());
    sbStadtStrStart_5->setValue(
                simurun_conf.value("sbStadtStrStart_5", 1850).toInt());
    sbStadtStrEnd_5->setValue(
                simurun_conf.value("sbStadtStrEnd_5", 2100).toInt());
    cbStadtStrBelag_6->setCurrentIndex(
                simurun_conf.value("city_road_6", 2).toInt());
    sbStadtStrStart_6->setValue(
                simurun_conf.value("sbStadtStrStart_6", 1850).toInt());
    sbStadtStrEnd_6->setValue(
                simurun_conf.value("sbStadtStrEnd_6", 2100).toInt());
    cbStadtStrBelag_7->setCurrentIndex(
                simurun_conf.value("city_road_7", 2).toInt());
    sbStadtStrStart_7->setValue(
                simurun_conf.value("sbStadtStrStart_7", 1850).toInt());
    sbStadtStrEnd_7->setValue(
                simurun_conf.value("sbStadtStrEnd_7", 2100).toInt());
    cbStadtStrBelag_8->setCurrentIndex(
                simurun_conf.value("city_road_8", 2).toInt());
    sbStadtStrStart_8->setValue(
                simurun_conf.value("sbStadtStrStart_8", 1850).toInt());
    sbStadtStrEnd_8->setValue(
                simurun_conf.value("sbStadtStrEnd_8", 2100).toInt());
    cbStadtStrBelag_9->setCurrentIndex(
                simurun_conf.value("city_road_9", 2).toInt());
    sbStadtStrStart_9->setValue(
                simurun_conf.value("sbStadtStrStart_9", 1850).toInt());
    sbStadtStrEnd_9->setValue(
                simurun_conf.value("sbStadtStrEnd_9", 2100).toInt());
    cbStadtStrBelag_10->setCurrentIndex(
                simurun_conf.value("city_road_10", 2).toInt());
    sbStadtStrStart_10->setValue(
                simurun_conf.value("sbStadtStrStart_10", 1850).toInt());
    sbStadtStrEnd_10->setValue(
                simurun_conf.value("sbStadtStrEnd_10", 2100).toInt());
    cbLandStrBelag_1->setCurrentIndex(
                simurun_conf.value("intercity_road_1", 3).toInt());
    sbLandStrStart_1->setValue(
                simurun_conf.value("sbLandStrStart_1", 1850).toInt());
    sbLandStrEnd_1->setValue(
                simurun_conf.value("sbLandStrEnd_1", 2100).toInt());
    cbLandStrBelag_2->setCurrentIndex(
                simurun_conf.value("intercity_road_2", 3).toInt());
    sbLandStrStart_2->setValue(
                simurun_conf.value("sbLandStrStart_2", 1850).toInt());
    sbLandStrEnd_2->setValue(
                simurun_conf.value("sbLandStrEnd_2", 2100).toInt());
    cbLandStrBelag_3->setCurrentIndex(
                simurun_conf.value("intercity_road_3", 3).toInt());
    sbLandStrStart_3->setValue(
                simurun_conf.value("sbLandStrStart_3", 1850).toInt());
    sbLandStrEnd_3->setValue(
                simurun_conf.value("sbLandStrEnd_3", 2100).toInt());
    cbLandStrBelag_4->setCurrentIndex(
                simurun_conf.value("intercity_road_4", 3).toInt());
    sbLandStrStart_4->setValue(
                simurun_conf.value("sbLandStrStart_4", 1850).toInt());
    sbLandStrEnd_4->setValue(
                simurun_conf.value("sbLandStrEnd_4", 2100).toInt());
    cbLandStrBelag_5->setCurrentIndex(
                simurun_conf.value("intercity_road_5", 3).toInt());
    sbLandStrStart_5->setValue(
                simurun_conf.value("sbLandStrStart_5", 1850).toInt());
    sbLandStrEnd_5->setValue(
                simurun_conf.value("sbLandStrEnd_5", 2100).toInt());
    cbLandStrBelag_6->setCurrentIndex(
                simurun_conf.value("intercity_road_6", 3).toInt());
    sbLandStrStart_6->setValue(
                simurun_conf.value("sbLandStrStart_6", 1850).toInt());
    sbLandStrEnd_6->setValue(
                simurun_conf.value("sbLandStrEnd_6", 2100).toInt());
    cbLandStrBelag_7->setCurrentIndex(
                simurun_conf.value("intercity_road_7", 3).toInt());
    sbLandStrStart_7->setValue(
                simurun_conf.value("sbLandStrStart_7", 1850).toInt());
    sbLandStrEnd_7->setValue(
                simurun_conf.value("sbLandStrEnd_7", 2100).toInt());
    cbLandStrBelag_8->setCurrentIndex(
                simurun_conf.value("intercity_road_8", 3).toInt());
    sbLandStrStart_8->setValue(
                simurun_conf.value("sbLandStrStart_8", 1850).toInt());
    sbLandStrEnd_8->setValue(
                simurun_conf.value("sbLandStrEnd_8", 2100).toInt());
    cbLandStrBelag_9->setCurrentIndex(
                simurun_conf.value("intercity_road_9", 3).toInt());
    sbLandStrStart_9->setValue(
                simurun_conf.value("sbLandStrStart_9", 1850).toInt());
    sbLandStrEnd_9->setValue(
                simurun_conf.value("sbLandStrEnd_9", 2100).toInt());
    cbLandStrBelag_10->setCurrentIndex(
                simurun_conf.value("intercity_road_10", 3).toInt());
    sbLandStrStart_10->setValue(
                simurun_conf.value("sbLandStrStart_10", 1850).toInt());
    sbLandStrEnd_10->setValue(
                simurun_conf.value("sbLandStrEnd_10", 2100).toInt());

    cbTreesShow->setCurrentIndex(
                simurun_conf.value("no_tree", 1).toInt());
    if (simurun_conf.value("tree_climates", 4).toInt() == 0){
        cbTreesKlima->setCurrentIndex(0);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 1){
        cbTreesKlima->setCurrentIndex(1);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 2){
        cbTreesKlima->setCurrentIndex(2);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 4){
        cbTreesKlima->setCurrentIndex(3);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 8){
        cbTreesKlima->setCurrentIndex(4);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 16){
        cbTreesKlima->setCurrentIndex(5);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 32){
        cbTreesKlima->setCurrentIndex(6);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 64){
        cbTreesKlima->setCurrentIndex(7);
    }
    if (simurun_conf.value("tree_climates", 4).toInt() == 128){
        cbTreesKlima->setCurrentIndex(8);
    }
    sbNoTreesKlima->setValue(
                simurun_conf.value("no_tree_climates", 130).toInt());
    sbBaumFeld->setValue(
                simurun_conf.value("max_no_of_trees_on_square", 3).toInt());
    sbForrestSize->setValue(
                simurun_conf.value("forest_base_size", 36).toInt());
    sbMapDivisor->setValue(
                simurun_conf.value("forest_map_size_divisor",38).toInt());
    sbForrestDivisor->setValue(
                simurun_conf.value("forest_count_divisor", 16).toInt());
    sbTreesDichte->setValue(
                simurun_conf.value("forest_inverse_spare_tree_density", 400).toInt());
    sbFlussMenge->setValue(
                simurun_conf.value("river_number", 16).toInt());
    sbFlussMinLang->setValue(
                simurun_conf.value("river_min_length", 16).toInt());
    sbFlussMaxLang->setValue(
                simurun_conf.value("river_max_length", 320).toInt());
    simurun_conf.endGroup();

    simurun_conf.beginGroup("Simutrans");
    rbEinfach->setChecked(
                simurun_conf.value("Starting_Money_old", true).toBool());
    rbErweitert->setChecked(
                simurun_conf.value("Starting_Money_new", false).toBool());
    chb_2->setChecked(
                simurun_conf.value("sbStartkapital_2", false).toBool());
    chb_3->setChecked(
                simurun_conf.value("sbStartkapital_3", false).toBool());
    chb_4->setChecked(
                simurun_conf.value("sbStartkapital_4", false).toBool());
    chb_5->setChecked(
                simurun_conf.value("sbStartkapital_5", false).toBool());
    chb_6->setChecked(
                simurun_conf.value("sbStartkapital_6", false).toBool());
    chb_7->setChecked(
                simurun_conf.value("sbStartkapital_7", false).toBool());
    chb_8->setChecked(
                simurun_conf.value("sbStartkapital_8", false).toBool());
    chb_9->setChecked(
                simurun_conf.value("sbStartkapital_9", false).toBool());
    chb_10->setChecked(
                simurun_conf.value("sbStartkapital_10", false).toBool());
    rbStrAlt->setChecked(
                simurun_conf.value("Road_Type_old", true).toBool());
    rbStrNeu->setChecked(
                simurun_conf.value("Road_Type_new", false).toBool());
    chbStadtStr_2->setChecked(
                simurun_conf.value("chbStadtStr_2", false).toBool());
    chbStadtStr_3->setChecked(
                simurun_conf.value("chbStadtStr_3", false).toBool());
    chbStadtStr_4->setChecked(
                simurun_conf.value("chbStadtStr_4", false).toBool());
    chbStadtStr_5->setChecked(
                simurun_conf.value("chbStadtStr_5", false).toBool());
    chbStadtStr_6->setChecked(
                simurun_conf.value("chbStadtStr_6", false).toBool());
    chbStadtStr_7->setChecked(
                simurun_conf.value("chbStadtStr_7", false).toBool());
    chbStadtStr_8->setChecked(
                simurun_conf.value("chbStadtStr_8", false).toBool());
    chbStadtStr_9->setChecked(
                simurun_conf.value("chbStadtStr_9", false).toBool());
    chbStadtStr_10->setChecked(
                simurun_conf.value("chbStadtStr_10", false).toBool());
    chbLandStr_2->setChecked(
                simurun_conf.value("chbLandStr_2", false).toBool());
    chbLandStr_3->setChecked(
                simurun_conf.value("chbLandStr_3", false).toBool());
    chbLandStr_4->setChecked(
                simurun_conf.value("chbLandStr_4", false).toBool());
    chbLandStr_5->setChecked(
                simurun_conf.value("chbLandStr_5", false).toBool());
    chbLandStr_6->setChecked(
                simurun_conf.value("chbLandStr_6", false).toBool());
    chbLandStr_7->setChecked(
                simurun_conf.value("chbLandStr_7", false).toBool());
    chbLandStr_8->setChecked(
                simurun_conf.value("chbLandStr_8", false).toBool());
    chbLandStr_9->setChecked(
                simurun_conf.value("chbLandStr_9", false).toBool());
    chbLandStr_10->setChecked(
                simurun_conf.value("chbLandStr_10", false).toBool());
    chb_Ziele_2->setChecked(
                simurun_conf.value("chb_Ziele_2", false).toBool());
    chb_Ziele_3->setChecked(
                simurun_conf.value("chb_Ziele_3", false).toBool());
    chb_Ziele_4->setChecked(
                simurun_conf.value("chb_Ziele_4", false).toBool());
    chb_Ziele_5->setChecked(
                simurun_conf.value("chb_Ziele_5", false).toBool());
    chb_Ziele_6->setChecked(
                simurun_conf.value("chb_Ziele_6", false).toBool());
    chb_Ziele_7->setChecked(
                simurun_conf.value("chb_Ziele_7", false).toBool());
    chb_Ziele_8->setChecked(
                simurun_conf.value("chb_Ziele_8", false).toBool());
    chb_Ziele_9->setChecked(
                simurun_conf.value("chb_Ziele_9", false).toBool());
    chb_Ziele_10->setChecked(
                simurun_conf.value("chb_Ziele_10", false).toBool());
    groupBox_FieldHeight->setChecked(
                simurun_conf.value("Activate_FieldHeight", false).toBool());
    lineEdit_Screensize->setText(
                simurun_conf.value("Screensize").toString());
    simurun_conf.endGroup();

    if (cbBeginner->currentIndex() == 0) {
        spPreisfaktor->setEnabled(false);
    } else {
        spPreisfaktor->setEnabled(true);
    };

    if (rbEinfach->isChecked() == true) {
        gbErweitert->setVisible(false);
        gbEinfach->setVisible(true);
    };

    if (rbErweitert->isChecked() == true) {
        gbEinfach->setVisible(false);
        gbErweitert->setVisible(true);
    };

    if (rbStrAlt->isChecked() == true) {
        gbStrNeu->setVisible(false);
        gbStrAlt->setVisible(true);
    };

    if (rbStrNeu->isChecked() == true) {
        gbStrAlt->setVisible(false);
        gbStrNeu->setVisible(true);
    };

}

void ConfigureClass::writeSimutransSettings()
{
    if (cbBeginner->currentIndex() == 0) {
        spPreisfaktor->setEnabled(false);
    } else {
        spPreisfaktor->setEnabled(true);
    };

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           cbPakSets->currentText() + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup(cbPakSets->currentText());
    // PROGRAM STUFF
    simurun_conf.setValue("singleuser_install",
                          cbSingleUser->currentIndex());
    simurun_conf.setValue("max_route_steps",
                          sbRoutesteps->value());
    simurun_conf.setValue("first_beginner",
                          cbBeginner->currentIndex());
    simurun_conf.setValue("beginner_price_factor",
                          spPreisfaktor->value());
    simurun_conf.setValue("max_transfers",
                          spTransfers->value());
    simurun_conf.setValue("max_hops",
                          sbSpruenge->value());
    simurun_conf.setValue("station_coverage",
                          sbAbdeckungen->value());
    simurun_conf.setValue("minimum_city_distance",
                          sbAbstRath->value());
    simurun_conf.setValue("special_building_distance",
                          spinBoxAttractionDistance->value());

    //DISPLAY STUFF
    simurun_conf.setValue("random_pedestrians",
                          cbPassanten->currentIndex());
    simurun_conf.setValue("only_single_info",
                          cbInfo->currentIndex());
    simurun_conf.setValue("pedes_and_car_info",
                          cbInfoAutos->currentIndex());
    simurun_conf.setValue("numbered_stations",
                          cbNummeriert->currentIndex());
    simurun_conf.setValue("stop_pedestrians",
                          cbPassagiereStationen->currentIndex());
    simurun_conf.setValue("show_names",
                          cbStationsnamen->currentIndex());
    simurun_conf.setValue("tree_info",
                          cbBaumInfo->currentIndex());
    simurun_conf.setValue("townhall_info",
                          cbRathausInfo->currentIndex());
    simurun_conf.setValue("ground_info",
                          cbBodenInfo->currentIndex());
    simurun_conf.setValue("drive_left",
                          cbLinksfahren->currentIndex());
    simurun_conf.setValue("default_citycar_life",
                          sbKfzLebensdauer->value());
    simurun_conf.setValue("water_animation_ms",
                          sbWasser->value());
    simurun_conf.setValue("random_wildlife_probability",
                          sbTiere->value());
    simurun_conf.setValue("random_grounds_probability",
                          sbLandschaft->value());
    simurun_conf.setValue("citycar_level",
                          sbKfzLevel->value());
    simurun_conf.setValue("show_vehicle_states",
                          cB_Vehicle_Status->currentIndex());
    simurun_conf.setValue("visualize_schedule",
                          cB_Visualize_Schedule->currentIndex());
    simurun_conf.setValue("tile_height",
                          sB_Height_Z_Direction->value());
    simurun_conf.setValue("height_conversion_factor",
                          spinBox_height_conversion_factor->value());
    simurun_conf.setValue("draw_earth_border",
                          comboBoxDrawEarthBorder->currentIndex());
    simurun_conf.setValue("draw_outside_tile",
                          comboBoxDrawOutsideTile->currentIndex());
    simurun_conf.setValue("background_color",
                          spinBoxBackgroundColor->value());
    simurun_conf.setValue("show_delete_buttons",
                          comboBoxDeleteGame->currentIndex());
    simurun_conf.setValue("hide_rail_return_ticket",
                          comboBoxInvertedSchedule->currentIndex());
    simurun_conf.setValue("unprotect_abandoned_player_months",
                          spinBoxAbandonedPlayer->value());
    simurun_conf.setValue("remove_dummy_player_months",
                          spinBoxRemoveDeadPlayer->value());

    //WINDOW STUFF
    simurun_conf.setValue("window_buttons_right",
                          cbButtons->currentIndex());
    simurun_conf.setValue("window_frame_active",
                          cbRahmen->currentIndex());
    simurun_conf.setValue("show_tooltips",
                          cbToolTipps->currentIndex());
    simurun_conf.setValue("tooltip_delay",
                          sbTooltippsDelay->value());
    simurun_conf.setValue("tooltip_duration",
                          sbToolTippsDauer->value());

    // TIME STUFF
    simurun_conf.setValue("starting_year",
                          sbJahr->value());
    simurun_conf.setValue("starting_month",
                          sbMonate->value());
    simurun_conf.setValue("bits_per_month",
                          sbForwardSpeed->value());
    simurun_conf.setValue("use_timeline",
                          cbNeueKfz->currentIndex());
    simurun_conf.setValue("show_month",
                          cbDatumsFormat->currentIndex());

    // SYSTEM STUFF
    simurun_conf.setValue("saveformat",
                          cbSpeicherFormat->currentIndex());
    simurun_conf.setValue("autosave",
                          cbMonatIntervall->currentIndex());
    simurun_conf.setValue("add_player_name_to_message",
                          cbMessages->currentIndex());
    simurun_conf.setValue("frames_per_second",
                          sbFPS->value());
    simurun_conf.setValue("simple_drawing_tile_size",
                          sbSimpleDawTiles->value());
    simurun_conf.setValue("fast_forward",
                          sbFastForward->value());
    simurun_conf.setValue("threads",
                          spinBoxThreadsToUse->value());
    simurun_conf.setValue("comboBoxDrawingFast",
                          comboBoxDrawingFast->currentIndex());

    // NETWORK STUFF
    simurun_conf.setValue("server_frames_ahead",
                          sbDelay->value());
    simurun_conf.setValue("server_frames_per_step",
                          sbUpdates->value());
    simurun_conf.setValue("additional_client_frames_behind",
                          sbTimeDelay->value());
    simurun_conf.setValue("server_frames_between_checks",
                          sbClientCheck->value());
    simurun_conf.setValue("server_announce",
                          cbServerAnnounce->currentIndex());
    simurun_conf.setValue("server_announce_intervall",
                          sbAboIntervall->value());
    simurun_conf.setValue("server_name",
                          txtServerName->text());
    simurun_conf.setValue("server_comment",
                          txtKommentar->toPlainText());
    simurun_conf.setValue("server_admin_pw",
                          txtAdminPW->text());
    simurun_conf.setValue("pause_server_no_clients",
                          cbServerPause->currentIndex());
    simurun_conf.setValue("server_dns",
                          txtServerAdresse->text());
    simurun_conf.setValue("server_email",
                          txtServerEmail->text());
    simurun_conf.setValue("server_pakurl",
                          txtPakSetUrl->text());
    simurun_conf.setValue("server_infurl",
                          txtServerInfo->text());
    simurun_conf.setValue("Nickname",
                          txtNickName->text());
    simurun_conf.setValue("chat_transparency",
                          spinBoxChatTransparency->value());

    // FACTORY STUFF
    simurun_conf.setValue("crossconnect_factories_percentage",
                          sbProzent->value());
    simurun_conf.setValue("industry_increase_every",
                          sbErweitern->value());
    simurun_conf.setValue("just_in_time",
                          cbBeliefern->currentIndex());
    simurun_conf.setValue("maximum_intransit_percentage",
                          spinBoxMinAmount->value());
    simurun_conf.setValue("electric_promille",
                          sbPromille->value());
    simurun_conf.setValue("factory_enforce_demand",
                          cbFabrikAnforderung->currentIndex());
    simurun_conf.setValue("factory_spacing",
                          spinBoxMinFactorySpacing->value());
    simurun_conf.setValue("max_factory_spacing",
                          spinBoxMaxFactorySpacing->value());
    simurun_conf.setValue("max_factory_spacing_percentage",
                          spinBoxMaxFactorySpacingPercent->value());

    // GOODS/PASSENGER STUFF
    simurun_conf.setValue("factory_arrival_periods",
                          sbPassInterwal->value());
    simurun_conf.setValue("passenger_multiplier",
                          sbMultiplikatorPassagiere->value());
    simurun_conf.setValue("electricity_multiplier",
                          sbMultiplikatorEnergie->value());
    simurun_conf.setValue("mail_multiplier",
                          sbMultiplikatorPost->value());
    simurun_conf.setValue("goods_multiplier",
                          sbMultiplikatorGoods->value());
    simurun_conf.setValue("passenger_factor",
                          sbPassagierFoktor->value());
    simurun_conf.setValue("no_routing_over_overcrowded",
                          cbStationUeberfuellt->currentIndex());
    simurun_conf.setValue("factory_worker_percentage",
                          sbArbeiteraufkommen->value());
    simurun_conf.setValue("tourist_percentage",
                          sbTouristenaufkommen->value());
    simurun_conf.setValue("factory_worker_radius",
                          sbRadius->value());
    simurun_conf.setValue("factory_worker_minimum_towns",
                          sbMinCities->value());
    simurun_conf.setValue("avoid_overcrowding",
                          cbZielUeberfuellt->currentIndex());
    simurun_conf.setValue("pay_for_total_distance",
                          cbTransportCash->currentIndex());
    simurun_conf.setValue("factory_worker_maximum_towns",
                          sbMaxCities->value());
    simurun_conf.setValue("growthfactor_villages",
                          sbUnter_1000->value());
    simurun_conf.setValue("growthfactor_cities",
                          sb1000_10000->value());
    simurun_conf.setValue("growthfactor_capitals",
                          sbUeber_10000->value());
    simurun_conf.setValue("seperate_halt_capacities",
                          cbKapazitaeten->currentIndex());
    simurun_conf.setValue("city_short_range_percentage",
                          sbArbeiterKleinerRadius->value());
    simurun_conf.setValue("city_medium_range_percentage",
                          ArbeiterGrosserRadius->value());
    simurun_conf.setValue("city_short_range_radius",
                          sbStadtRadiusKurz->value());
    simurun_conf.setValue("city_medium_range_radius",
                          sbStadtRadiusMittel->value());
    simurun_conf.setValue("sb_Jahr_1",
                          sb_Jahr_1->value());
    simurun_conf.setValue("sb_Jahr_Ziel_1",
                          sb_Jahr_Ziel_1->value());
    simurun_conf.setValue("sb_Jahr_2",
                          sb_Jahr_2->value());
    simurun_conf.setValue("sb_Jahr_Ziel_2",
                          sb_Jahr_Ziel_2->value());
    simurun_conf.setValue("sb_Jahr_3",
                          sb_Jahr_3->value());
    simurun_conf.setValue("sb_Jahr_Ziel_3",
                          sb_Jahr_Ziel_3->value());
    simurun_conf.setValue("sb_Jahr_4",
                          sb_Jahr_4->value());
    simurun_conf.setValue("sb_Jahr_Ziel_4",
                          sb_Jahr_Ziel_4->value());
    simurun_conf.setValue("sb_Jahr_5",
                          sb_Jahr_5->value());
    simurun_conf.setValue("sb_Jahr_Ziel_5",
                          sb_Jahr_Ziel_5->value());
    simurun_conf.setValue("sb_Jahr_6",
                          sb_Jahr_6->value());
    simurun_conf.setValue("sb_Jahr_Ziel_6",
                          sb_Jahr_Ziel_6->value());
    simurun_conf.setValue("sb_Jahr_7",
                          sb_Jahr_7->value());
    simurun_conf.setValue("sb_Jahr_Ziel_7",
                          sb_Jahr_Ziel_7->value());
    simurun_conf.setValue("sb_Jahr_8",
                          sb_Jahr_8->value());
    simurun_conf.setValue("sb_Jahr_Ziel_8",
                          sb_Jahr_Ziel_8->value());
    simurun_conf.setValue("sb_Jahr_9",
                          sb_Jahr_9->value());
    simurun_conf.setValue("sb_Jahr_Ziel_9",
                          sb_Jahr_Ziel_9->value());
    simurun_conf.setValue("sb_Jahr_10",
                          sb_Jahr_10->value());
    simurun_conf.setValue("sb_Jahr_Ziel_10",
                          sb_Jahr_Ziel_10->value());

    // MONEY STUFF
    simurun_conf.setValue("left_to_right_graphs",
                          cbStatistik->currentIndex());
    simurun_conf.setValue("allow_buying_obsolete_vehicles",
                          cbAusgemustert->currentIndex());
    simurun_conf.setValue("used_vehicle_reduction",
                          sbWertverlustKfz->value());
    // Starting money Simutrans 102.2.2 and before
    simurun_conf.setValue("starting_money",
                          sbStartkapital->value());

    // Starting money since Simutrans 110.0 and higher.
    simurun_conf.setValue("Startkapital_1",
                          sbStartkapital_1->value());
    simurun_conf.setValue("StartJahr_1",
                          sbStartJahr_1->value());
    simurun_conf.setValue("Interpoliert_1",
                          cbInterpoliert_1->currentIndex());
    simurun_conf.setValue("Startkapital_2",
                          sbStartkapital_2->value());
    simurun_conf.setValue("StartJahr_2",
                          sbStartJahr_2->value());
    simurun_conf.setValue("Interpoliert_2",
                          cbInterpoliert_2->currentIndex());
    simurun_conf.setValue("Startkapital_3",
                          sbStartkapital_3->value());
    simurun_conf.setValue("StartJahr_3",
                          sbStartJahr_3->value());
    simurun_conf.setValue("Interpoliert_3",
                          cbInterpoliert_3->currentIndex());
    simurun_conf.setValue("Startkapital_4",
                          sbStartkapital_4->value());
    simurun_conf.setValue("StartJahr_4",
                          sbStartJahr_4->value());
    simurun_conf.setValue("Interpoliert_4",
                          cbInterpoliert_4->currentIndex());
    simurun_conf.setValue("Startkapital_5",
                          sbStartkapital_5->value());
    simurun_conf.setValue("StartJahr_5",
                          sbStartJahr_5->value());
    simurun_conf.setValue("Interpoliert_5",
                          cbInterpoliert_5->currentIndex());
    simurun_conf.setValue("Startkapital_6",
                          sbStartkapital_6->value());
    simurun_conf.setValue("StartJahr_6",
                          sbStartJahr_6->value());
    simurun_conf.setValue("Interpoliert_6",
                          cbInterpoliert_6->currentIndex());
    simurun_conf.setValue("Startkapital_7",
                          sbStartkapital_7->value());
    simurun_conf.setValue("StartJahr_7",
                          sbStartJahr_7->value());
    simurun_conf.setValue("Interpoliert_7",
                          cbInterpoliert_7->currentIndex());
    simurun_conf.setValue("Startkapital_8",
                          sbStartkapital_8->value());
    simurun_conf.setValue("StartJahr_8",
                          sbStartJahr_8->value());
    simurun_conf.setValue("Interpoliert_8",
                          cbInterpoliert_8->currentIndex());
    simurun_conf.setValue("Startkapital_9",
                          sbStartkapital_9->value());
    simurun_conf.setValue("StartJahr_9",
                          sbStartJahr_9->value());
    simurun_conf.setValue("Interpoliert_9",
                          cbInterpoliert_9->currentIndex());
    simurun_conf.setValue("Startkapital_10",
                          sbStartkapital_10->value());
    simurun_conf.setValue("StartJahr_10",
                          sbStartJahr_10->value());
    simurun_conf.setValue("Interpoliert_10",
                          cbInterpoliert_10->currentIndex());
    simurun_conf.setValue("maintenance_building",
                          sbGebaeude->value());
    simurun_conf.setValue("cost_multiply_dock",
                          sbSchiffsdock->value());
    simurun_conf.setValue("cost_multiply_station",
                          sbBahnhof->value());
    simurun_conf.setValue("cost_multiply_roadstop",
                          sbHaltestelle->value());
    simurun_conf.setValue("cost_multiply_airterminal",
                          sbTerminal->value());
    simurun_conf.setValue("cost_multiply_post",
                          sbPosthalt->value());
    simurun_conf.setValue("cost_multiply_headquarter",
                          sbHauptquartier->value());
    simurun_conf.setValue("cost_depot_air",
                          sbHanger->value());
    simurun_conf.setValue("cost_depot_rail",
                          sbLockschuppen->value());
    simurun_conf.setValue("cost_depot_road",
                          sbKfzdepot->value());
    simurun_conf.setValue("cost_depot_ship",
                          sbSchiffsdepot->value());
    simurun_conf.setValue("cost_signal",
                          sbSignale->value());
    simurun_conf.setValue("cost_tunnel",
                          sbTunnel->value());
    simurun_conf.setValue("cost_third_rail",
                          sbDrittesGleis->value());
    simurun_conf.setValue("cost_buy_land",
                          sbLanderweiterung->value());
    simurun_conf.setValue("cost_alter_land",
                          sbLandaenderung->value());
    simurun_conf.setValue("cost_set_slope",
                          sbSlope->value());
    simurun_conf.setValue("cost_found_city",
                          sbNeueStadt->value());
    simurun_conf.setValue("cost_multiply_found_industry",
                          sbIndustrie->value());
    simurun_conf.setValue("cost_remove_tree",
                          sbBaumFaellen->value());
    simurun_conf.setValue("cost_multiply_remove_haus",
                          sbAbreissen->value());
    simurun_conf.setValue("cost_multiply_remove_field",
                          sbFeldEntfernen->value());
    simurun_conf.setValue("cost_transformer",
                          sbTrafoKaufen->value());
    simurun_conf.setValue("cost_maintain_transformer",
                          sbTrafoWartung->value());
    simurun_conf.setValue("toll_runningcost_percentage",
                          sb_Maut->value());
    simurun_conf.setValue("toll_waycost_percentage",
                          sb_StrKosten->value());
    simurun_conf.setValue("bonus_basefactor",
                          sb_Bonus_Basefactor->value());


    // ROAD/RIVER/FORREST STUFF
    simurun_conf.setValue("intercity_road_length",
                          sbLandStrLaenge->value());
    simurun_conf.setValue("straight_way_without_control",
                          cbStraightWays->currentIndex());
    // Road settings Simutrans 102.2.2 and before.
    simurun_conf.setValue("city_road_type",
                          cbStadtStrBelag->currentIndex());
    simurun_conf.setValue("intercity_road_type",
                          cbLandStrBelag->currentIndex());
    // Road settings since Simutrans 110.0 and higher.
    simurun_conf.setValue("city_road_1",
                          cbStadtStrBelag_1->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_1",
                          sbStadtStrStart_1->value());
    simurun_conf.setValue("sbStadtStrEnd_1",
                          sbStadtStrEnd_1->value());
    simurun_conf.setValue("city_road_2",
                          cbStadtStrBelag_2->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_2",
                          sbStadtStrStart_2->value());
    simurun_conf.setValue("sbStadtStrEnd_2",
                          sbStadtStrEnd_2->value());
    simurun_conf.setValue("city_road_3",
                          cbStadtStrBelag_3->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_3",
                          sbStadtStrStart_3->value());
    simurun_conf.setValue("sbStadtStrEnd_3",
                          sbStadtStrEnd_3->value());
    simurun_conf.setValue("city_road_4",
                          cbStadtStrBelag_4->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_4",
                          sbStadtStrStart_4->value());
    simurun_conf.setValue("sbStadtStrEnd_4",
                          sbStadtStrEnd_4->value());
    simurun_conf.setValue("city_road_5",
                          cbStadtStrBelag_5->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_5",
                          sbStadtStrStart_5->value());
    simurun_conf.setValue("sbStadtStrEnd_5",
                          sbStadtStrEnd_5->value());
    simurun_conf.setValue("city_road_6",
                          cbStadtStrBelag_6->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_6",
                          sbStadtStrStart_6->value());
    simurun_conf.setValue("sbStadtStrEnd_6",
                          sbStadtStrEnd_6->value());
    simurun_conf.setValue("city_road_7",
                          cbStadtStrBelag_7->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_7",
                          sbStadtStrStart_7->value());
    simurun_conf.setValue("sbStadtStrEnd_7",
                          sbStadtStrEnd_7->value());
    simurun_conf.setValue("city_road_8",
                          cbStadtStrBelag_8->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_8",
                          sbStadtStrStart_8->value());
    simurun_conf.setValue("sbStadtStrEnd_8",
                          sbStadtStrEnd_8->value());
    simurun_conf.setValue("city_road_9",
                          cbStadtStrBelag_9->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_9",
                          sbStadtStrStart_9->value());
    simurun_conf.setValue("sbStadtStrEnd_9",
                          sbStadtStrEnd_9->value());
    simurun_conf.setValue("city_road_10",
                          cbStadtStrBelag_10->currentIndex());
    simurun_conf.setValue("sbStadtStrStart_10",
                          sbStadtStrStart_10->value());
    simurun_conf.setValue("sbStadtStrEnd_10",
                          sbStadtStrEnd_10->value());

    simurun_conf.setValue("intercity_road_1",
                          cbLandStrBelag_1->currentIndex());
    simurun_conf.setValue("sbLandStrStart_1",
                          sbLandStrStart_1->value());
    simurun_conf.setValue("sbLandStrEnd_1",
                          sbLandStrEnd_1->value());
    simurun_conf.setValue("intercity_road_2",
                          cbLandStrBelag_2->currentIndex());
    simurun_conf.setValue("sbLandStrStart_2",
                          sbLandStrStart_2->value());
    simurun_conf.setValue("sbLandStrEnd_2",
                          sbLandStrEnd_2->value());
    simurun_conf.setValue("intercity_road_3",
                          cbLandStrBelag_3->currentIndex());
    simurun_conf.setValue("sbLandStrStart_3",
                          sbLandStrStart_3->value());
    simurun_conf.setValue("sbLandStrEnd_3",
                          sbLandStrEnd_3->value());
    simurun_conf.setValue("intercity_road_4",
                          cbLandStrBelag_4->currentIndex());
    simurun_conf.setValue("sbLandStrStart_4",
                          sbLandStrStart_4->value());
    simurun_conf.setValue("sbLandStrEnd_4",
                          sbLandStrEnd_4->value());
    simurun_conf.setValue("intercity_road_5",
                          cbLandStrBelag_5->currentIndex());
    simurun_conf.setValue("sbLandStrStart_5",
                          sbLandStrStart_5->value());
    simurun_conf.setValue("sbLandStrEnd_5",
                          sbLandStrEnd_5->value());
    simurun_conf.setValue("intercity_road_6",
                          cbLandStrBelag_6->currentIndex());
    simurun_conf.setValue("sbLandStrStart_6",
                          sbLandStrStart_6->value());
    simurun_conf.setValue("sbLandStrEnd_6",
                          sbLandStrEnd_6->value());
    simurun_conf.setValue("intercity_road_7",
                          cbLandStrBelag_7->currentIndex());
    simurun_conf.setValue("sbLandStrStart_7",
                          sbLandStrStart_7->value());
    simurun_conf.setValue("sbLandStrEnd_7",
                          sbLandStrEnd_7->value());
    simurun_conf.setValue("intercity_road_8",
                          cbLandStrBelag_8->currentIndex());
    simurun_conf.setValue("sbLandStrStart_8",
                          sbLandStrStart_8->value());
    simurun_conf.setValue("sbLandStrEnd_8",
                          sbLandStrEnd_8->value());
    simurun_conf.setValue("intercity_road_9",
                          cbLandStrBelag_9->currentIndex());
    simurun_conf.setValue("sbLandStrStart_9",
                          sbLandStrStart_9->value());
    simurun_conf.setValue("sbLandStrEnd_9",
                          sbLandStrEnd_9->value());
    simurun_conf.setValue("intercity_road_10",
                          cbLandStrBelag_10->currentIndex());
    simurun_conf.setValue("sbLandStrStart_10",
                          sbLandStrStart_10->value());
    simurun_conf.setValue("sbLandStrEnd_10",
                          sbLandStrEnd_10->value());
    simurun_conf.setValue("no_tree",
                          cbTreesShow->currentIndex());
    if (cbTreesKlima->currentIndex() == 0){
        simurun_conf.setValue("tree_climates",
                              0);
    }
    if (cbTreesKlima->currentIndex() == 1){
        simurun_conf.setValue("tree_climates",
                              1);
    }
    if (cbTreesKlima->currentIndex() == 2){
        simurun_conf.setValue("tree_climates",
                              2);
    }
    if (cbTreesKlima->currentIndex() == 3){
        simurun_conf.setValue("tree_climates",
                              4);
    }
    if (cbTreesKlima->currentIndex() == 4){
        simurun_conf.setValue("tree_climates",
                              8);
    }
    if (cbTreesKlima->currentIndex() == 5){
        simurun_conf.setValue("tree_climates",
                              16);
    }
    if (cbTreesKlima->currentIndex() == 6){
        simurun_conf.setValue("tree_climates",
                              32);
    }
    if (cbTreesKlima->currentIndex() == 7){
        simurun_conf.setValue("tree_climates",
                              64);
    }
    if (cbTreesKlima->currentIndex() == 8){
        simurun_conf.setValue("tree_climates",
                              128);
    }

    simurun_conf.setValue("no_tree_climates",
                          sbNoTreesKlima->value());
    simurun_conf.setValue("max_no_of_trees_on_square",
                          sbBaumFeld->value());
    simurun_conf.setValue("forest_base_size",
                          sbForrestSize->value());
    simurun_conf.setValue("forest_map_size_divisor",
                          sbMapDivisor->value());
    simurun_conf.setValue("forest_count_divisor",
                          sbForrestDivisor->value());
    simurun_conf.setValue("forest_inverse_spare_tree_density",
                          sbTreesDichte->value());
    simurun_conf.setValue("river_number",
                          sbFlussMenge->value());
    simurun_conf.setValue("river_min_length",
                          sbFlussMinLang->value());
    simurun_conf.setValue("river_max_length",
                          sbFlussMaxLang->value());

    simurun_conf.endGroup();

    simurun_conf.beginGroup("Simutrans");
    simurun_conf.setValue("Starting_Money_old", rbEinfach->isChecked());
    simurun_conf.setValue("Starting_Money_new", rbErweitert->isChecked());
    simurun_conf.setValue("sbStartkapital_2", chb_2->isChecked());
    simurun_conf.setValue("sbStartkapital_3", chb_3->isChecked());
    simurun_conf.setValue("sbStartkapital_4", chb_4->isChecked());
    simurun_conf.setValue("sbStartkapital_5", chb_5->isChecked());
    simurun_conf.setValue("sbStartkapital_6", chb_6->isChecked());
    simurun_conf.setValue("sbStartkapital_7", chb_7->isChecked());
    simurun_conf.setValue("sbStartkapital_8", chb_8->isChecked());
    simurun_conf.setValue("sbStartkapital_9", chb_9->isChecked());
    simurun_conf.setValue("sbStartkapital_10", chb_10->isChecked());
    simurun_conf.setValue("Road_Type_old", rbStrAlt->isChecked());
    simurun_conf.setValue("Road_Type_new", rbStrNeu->isChecked());
    simurun_conf.setValue("chbStadtStr_2", chbStadtStr_2->isChecked());
    simurun_conf.setValue("chbStadtStr_3", chbStadtStr_3->isChecked());
    simurun_conf.setValue("chbStadtStr_4", chbStadtStr_4->isChecked());
    simurun_conf.setValue("chbStadtStr_5", chbStadtStr_5->isChecked());
    simurun_conf.setValue("chbStadtStr_6", chbStadtStr_6->isChecked());
    simurun_conf.setValue("chbStadtStr_7", chbStadtStr_7->isChecked());
    simurun_conf.setValue("chbStadtStr_8", chbStadtStr_8->isChecked());
    simurun_conf.setValue("chbStadtStr_9", chbStadtStr_9->isChecked());
    simurun_conf.setValue("chbStadtStr_10", chbStadtStr_10->isChecked());
    simurun_conf.setValue("chbLandStr_2", chbLandStr_2->isChecked());
    simurun_conf.setValue("chbLandStr_3", chbLandStr_3->isChecked());
    simurun_conf.setValue("chbLandStr_4", chbLandStr_4->isChecked());
    simurun_conf.setValue("chbLandStr_5", chbLandStr_5->isChecked());
    simurun_conf.setValue("chbLandStr_6", chbLandStr_6->isChecked());
    simurun_conf.setValue("chbLandStr_7", chbLandStr_7->isChecked());
    simurun_conf.setValue("chbLandStr_8", chbLandStr_8->isChecked());
    simurun_conf.setValue("chbLandStr_9", chbLandStr_9->isChecked());
    simurun_conf.setValue("chbLandStr_10", chbLandStr_10->isChecked());
    simurun_conf.setValue("chb_Ziele_2", chb_Ziele_2->isChecked());
    simurun_conf.setValue("chb_Ziele_3", chb_Ziele_3->isChecked());
    simurun_conf.setValue("chb_Ziele_4", chb_Ziele_4->isChecked());
    simurun_conf.setValue("chb_Ziele_5", chb_Ziele_5->isChecked());
    simurun_conf.setValue("chb_Ziele_6", chb_Ziele_6->isChecked());
    simurun_conf.setValue("chb_Ziele_7", chb_Ziele_7->isChecked());
    simurun_conf.setValue("chb_Ziele_8", chb_Ziele_8->isChecked());
    simurun_conf.setValue("chb_Ziele_9", chb_Ziele_9->isChecked());
    simurun_conf.setValue("chb_Ziele_10", chb_Ziele_10->isChecked());
    simurun_conf.setValue("Activate_FieldHeight", groupBox_FieldHeight->isChecked());
    simurun_conf.setValue("Screensize", lineEdit_Screensize->text());
    simurun_conf.endGroup();
}

void ConfigureClass::loadDefaultValues()
{

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Auf Standardwerte setzen"));
    msgBox.setText(tr("<b>Einstellungen auf Standardwerte"
                          " zurücksetzen?</b>"));
    msgBox.setInformativeText(tr("Alle aktuell angezeigten Werte,"
                                     " werden auf die Standardeinstellungen"
                                     " zurück gesetzt."));
    msgBox.setDetailedText(tr("Dies betrifft nur die aktuell angezeigten"
                                  " Einstellungen, die Werte in den anderen"
                                  " Kategorien werden nicht verändert."));

    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        if (tabMain->currentIndex() == 0) {

            // Simutrans Starter Seetings
            cbStaedtenamen->setCurrentIndex(0);
            LanguageAppGroup->setChecked(false);
            cbSprachen->setCurrentIndex(0);
            cbIconDesktop->setChecked(false);
        }
        if (tabSettings->currentIndex() == 0) {

            // PROGRAM STUFF
            cbSingleUser->setCurrentIndex(0);
            sbRoutesteps->setValue(100000);
            cbBeginner->setCurrentIndex(0);
            spPreisfaktor->setValue(1500);
            spTransfers->setValue(9);
            sbSpruenge->setValue(2000);
            sbAbdeckungen->setValue(2);
            sbAbstRath->setValue(16);
            spinBoxAttractionDistance->setValue(3);

            //DISPLAY STUFF
            cbPassanten->setCurrentIndex(1);
            cbInfo->setCurrentIndex(1);
            cbInfoAutos->setCurrentIndex(1);
            cbNummeriert->setCurrentIndex(0);
            cbPassagiereStationen->setCurrentIndex(1);
            cbStationsnamen->setCurrentIndex(3);
            cbBaumInfo->setCurrentIndex(1);
            cbRathausInfo->setCurrentIndex(1);
            cbBodenInfo->setCurrentIndex(1);
            cbLinksfahren->setCurrentIndex(0);
            sbKfzLebensdauer->setValue(36);
            sbWasser->setValue(250);
            sbTiere->setValue(1000);
            sbLandschaft->setValue(10);
            sbKfzLevel->setValue(5);
            cB_Visualize_Schedule->setCurrentIndex(0);
            cB_Vehicle_Status->setCurrentIndex(1);
            sB_Height_Z_Direction->setValue(8);
            groupBox_FieldHeight->setChecked(false);
            spinBox_height_conversion_factor->setValue(4);
            comboBoxInvertedSchedule->setCurrentIndex(1);
            comboBoxDeleteGame->setCurrentIndex(0);
            spinBoxAbandonedPlayer->setValue(0);
            spinBoxRemoveDeadPlayer->setValue(6);

            //WINDOW STUFF
            cbButtons->setCurrentIndex(0);
            cbRahmen->setCurrentIndex(0);
            cbToolTipps->setCurrentIndex(1);
            sbTooltippsDelay->setValue(500);
            sbToolTippsDauer->setValue(1500);

            // TIME STUFF
            sbJahr->setValue(1930);
            sbMonate->setValue(1);
            sbForwardSpeed->setValue(20);
            cbNeueKfz->setCurrentIndex(3);
            cbDatumsFormat->setCurrentIndex(1);

            // SYSTEM STUFF
            cbSpeicherFormat->setCurrentIndex(0);
            cbMonatIntervall->setCurrentIndex(0);
            cbMessages->setCurrentIndex(1);
            sbFPS->setValue(25);
            sbSimpleDawTiles->setValue(24);
            sbFastForward->setValue(100);
            spinBoxThreadsToUse->setValue(4);
            comboBoxDrawingFast->setCurrentIndex(1);
        };

        if (tabSettings->currentIndex() == 1) {
            // NETWORK STUFF
            sbDelay->setValue(4);
            sbUpdates->setValue(4);
            sbTimeDelay->setValue(0);
            sbClientCheck->setValue(256);
            cbServerAnnounce->setCurrentIndex(0);
            sbAboIntervall->setValue(900);
            txtServerName->setText("");
            txtKommentar->setPlainText("");
            txtAdminPW->setText("");
            txtServerAdresse->setText("");
            txtServerEmail->setText("");
            txtPakSetUrl->setText("");
            txtServerInfo->setText("");
            cbServerPause->setCurrentIndex(1);
            spinBoxChatTransparency->setValue(75);
        };

        if (tabSettings->currentIndex() == 2) {
            // FACTORY STUFF
            sbProzent->setValue(33);
            sbErweitern->setValue(2000);
            cbBeliefern->setCurrentIndex(1);
            sbPromille->setValue(330);
            cbFabrikAnforderung->setCurrentIndex(1);
            spinBoxMinAmount->setValue(0);
            spinBoxMinFactorySpacing->setValue(6);
            spinBoxMaxFactorySpacing->setValue(40);
            spinBoxMaxFactorySpacingPercent->setValue(25);

            // GOODS/PASSENGER STUFF
            sbPassInterwal->setValue(4);
            sbMultiplikatorPassagiere->setValue(40);
            sbMultiplikatorEnergie->setValue(0);
            sbMultiplikatorPost->setValue(20);
            sbMultiplikatorGoods->setValue(20);
            sbPassagierFoktor->setValue(10);
            cbStationUeberfuellt->setCurrentIndex(0);
            sbArbeiteraufkommen->setValue(33);
            sbTouristenaufkommen->setValue(16);
            sbRadius->setValue(77);
            sbMinCities->setValue(2);
            cbZielUeberfuellt->setCurrentIndex(0);
            cbTransportCash->setCurrentIndex(0);
            sbMaxCities->setValue(4);
            sbUnter_1000->setValue(400);
            sb1000_10000->setValue(200);
            sbUeber_10000->setValue(100);
            cbKapazitaeten->setCurrentIndex(0);
            sbArbeiterKleinerRadius->setValue(60);
            ArbeiterGrosserRadius->setValue(30);
            sbStadtRadiusKurz->setValue(50);
            sbStadtRadiusMittel->setValue(100);
            chb_Ziele_2->setChecked(false);
            chb_Ziele_3->setChecked(false);
            chb_Ziele_4->setChecked(false);
            chb_Ziele_5->setChecked(false);
            chb_Ziele_6->setChecked(false);
            chb_Ziele_7->setChecked(false);
            chb_Ziele_8->setChecked(false);
            chb_Ziele_9->setChecked(false);
            chb_Ziele_10->setChecked(false);
            sb_Jahr_1->setValue(1835);
            sb_Jahr_Ziel_1->setValue(1);
            sb_Jahr_2->setValue(1835);
            sb_Jahr_Ziel_2->setValue(1);
            sb_Jahr_3->setValue(1835);
            sb_Jahr_Ziel_3->setValue(1);
            sb_Jahr_4->setValue(1835);
            sb_Jahr_Ziel_4->setValue(1);
            sb_Jahr_5->setValue(1835);
            sb_Jahr_Ziel_5->setValue(1);
            sb_Jahr_6->setValue(1835);
            sb_Jahr_Ziel_6->setValue(1);
            sb_Jahr_7->setValue(1835);
            sb_Jahr_Ziel_7->setValue(1);
            sb_Jahr_8->setValue(1835);
            sb_Jahr_Ziel_8->setValue(1);
            sb_Jahr_9->setValue(1835);
            sb_Jahr_Ziel_9->setValue(1);
            sb_Jahr_10->setValue(1835);
            sb_Jahr_Ziel_10->setValue(1);
        };

        if (tabSettings->currentIndex() == 3) {
            // MONEY STUFF
            rbEinfach->setChecked(true);
            gbErweitert->setVisible(false);
            gbEinfach->setVisible(true);
            rbErweitert->setChecked(false);
            chb_2->setChecked(false);
            chb_3->setChecked(false);
            chb_4->setChecked(false);
            chb_5->setChecked(false);
            chb_6->setChecked(false);
            chb_7->setChecked(false);
            chb_8->setChecked(false);
            chb_9->setChecked(false);
            chb_10->setChecked(false);
            cbStatistik->setCurrentIndex(0);
            cbAusgemustert->setCurrentIndex(1);
            sbWertverlustKfz->setValue(0);
            sbStartkapital->setValue(2000000);
            sbStartkapital_1->setValue(2000000);
            sbStartJahr_1->setValue(1850);
            cbInterpoliert_1->setCurrentIndex(1);
            sbStartkapital_2->setValue(2000000);
            sbStartJahr_2->setValue(1850);
            cbInterpoliert_2->setCurrentIndex(1);
            sbStartkapital_3->setValue(2000000);
            sbStartJahr_3->setValue(1850);
            cbInterpoliert_3->setCurrentIndex(1);
            sbStartkapital_4->setValue(2000000);
            sbStartJahr_4->setValue(1850);
            cbInterpoliert_4->setCurrentIndex(1);
            sbStartkapital_5->setValue(2000000);
            sbStartJahr_5->setValue(1850);
            cbInterpoliert_5->setCurrentIndex(1);
            sbStartkapital_6->setValue(2000000);
            sbStartJahr_6->setValue(1850);
            cbInterpoliert_6->setCurrentIndex(1);
            sbStartkapital_7->setValue(2000000);
            sbStartJahr_7->setValue(1850);
            cbInterpoliert_7->setCurrentIndex(1);
            sbStartkapital_8->setValue(2000000);
            sbStartJahr_8->setValue(1850);
            cbInterpoliert_8->setCurrentIndex(1);
            sbStartkapital_9->setValue(2000000);
            sbStartJahr_9->setValue(1850);
            cbInterpoliert_9->setCurrentIndex(1);
            sbStartkapital_10->setValue(2000000);
            sbStartJahr_10->setValue(1850);
            cbInterpoliert_10->setCurrentIndex(1);
            sbGebaeude->setValue(2000);
            sbSchiffsdock->setValue(500);
            sbBahnhof->setValue(600);
            sbHaltestelle->setValue(400);
            sbTerminal->setValue(3000);
            sbPosthalt->setValue(300);
            sbHauptquartier->setValue(1000);
            sbHanger->setValue(5000);
            sbLockschuppen->setValue(1000);
            sbKfzdepot->setValue(1300);
            sbSchiffsdepot->setValue(2500);
            sbSignale->setValue(500);
            sbTunnel->setValue(10000);
            sbDrittesGleis->setValue(80);
            sbLanderweiterung->setValue(100);
            sbLandaenderung->setValue(1000);
            sbSlope->setValue(2500);
            sbNeueStadt->setValue(5000000);
            sbIndustrie->setValue(20000);
            sbBaumFaellen->setValue(100);
            sbAbreissen->setValue(1000);
            sbFeldEntfernen->setValue(5000);
            sbTrafoKaufen->setValue(2500);
            sbTrafoWartung->setValue(20);
            sb_Maut->setValue(0);
            sb_StrKosten->setValue(0);
            sb_Bonus_Basefactor->setValue(125);
        };

        if (tabSettings->currentIndex() == 4) {
            // ROAD/RIVER/FORREST STUFF
            sbLandStrLaenge->setValue(200);
            rbStrAlt->setChecked(true);
            rbStrNeu->setChecked(false);
            chbStadtStr_2->setChecked(false);
            chbStadtStr_3->setChecked(false);
            chbStadtStr_4->setChecked(false);
            chbStadtStr_5->setChecked(false);
            chbStadtStr_6->setChecked(false);
            chbStadtStr_7->setChecked(false);
            chbStadtStr_8->setChecked(false);
            chbStadtStr_9->setChecked(false);
            chbStadtStr_10->setChecked(false);
            chbLandStr_2->setChecked(false);
            chbLandStr_3->setChecked(false);
            chbLandStr_4->setChecked(false);
            chbLandStr_5->setChecked(false);
            chbLandStr_6->setChecked(false);
            chbLandStr_7->setChecked(false);
            chbLandStr_8->setChecked(false);
            chbLandStr_9->setChecked(false);
            chbLandStr_10->setChecked(false);
            // Road settings Simutrans 102.2.2 and before.
            cbStadtStrBelag->setCurrentIndex(2);
            cbLandStrBelag->setCurrentIndex(3);
            // Road settings since Simutrans 110.0 and higher.
            cbStadtStrBelag_1->setCurrentIndex(2);
            sbStadtStrStart_1->setValue(1850);
            sbStadtStrEnd_1->setValue(2100);
            cbStadtStrBelag_2->setCurrentIndex(2);
            sbStadtStrStart_2->setValue(1850);
            sbStadtStrEnd_2->setValue(2100);
            cbStadtStrBelag_3->setCurrentIndex(2);
            sbStadtStrStart_3->setValue(1850);
            sbStadtStrEnd_3->setValue(2100);
            cbStadtStrBelag_4->setCurrentIndex(2);
            sbStadtStrStart_4->setValue(1850);
            sbStadtStrEnd_4->setValue(2100);
            cbStadtStrBelag_5->setCurrentIndex(2);
            sbStadtStrStart_5->setValue(1850);
            sbStadtStrEnd_5->setValue(2100);
            cbStadtStrBelag_6->setCurrentIndex(2);
            sbStadtStrStart_6->setValue(1850);
            sbStadtStrEnd_6->setValue(2100);
            cbStadtStrBelag_7->setCurrentIndex(2);
            sbStadtStrStart_7->setValue(1850);
            sbStadtStrEnd_7->setValue(2100);
            cbStadtStrBelag_8->setCurrentIndex(2);
            sbStadtStrStart_8->setValue(1850);
            sbStadtStrEnd_8->setValue(2100);
            cbStadtStrBelag_9->setCurrentIndex(2);
            sbStadtStrStart_9->setValue(1850);
            sbStadtStrEnd_9->setValue(2100);
            cbStadtStrBelag_10->setCurrentIndex(2);
            sbStadtStrStart_10->setValue(1850);
            sbStadtStrEnd_10->setValue(2100);
            cbLandStrBelag_1->setCurrentIndex(3);
            sbLandStrStart_1->setValue(1850);
            sbLandStrEnd_1->setValue(2100);
            cbLandStrBelag_2->setCurrentIndex(3);
            sbLandStrStart_2->setValue(1850);
            sbLandStrEnd_2->setValue(2100);
            cbLandStrBelag_3->setCurrentIndex(3);
            sbLandStrStart_3->setValue(1850);
            sbLandStrEnd_3->setValue(2100);
            cbLandStrBelag_4->setCurrentIndex(3);
            sbLandStrStart_4->setValue(1850);
            sbLandStrEnd_4->setValue(2100);
            cbLandStrBelag_5->setCurrentIndex(3);
            sbLandStrStart_5->setValue(1850);
            sbLandStrEnd_5->setValue(2100);
            cbLandStrBelag_6->setCurrentIndex(3);
            sbLandStrStart_6->setValue(1850);
            sbLandStrEnd_6->setValue(2100);
            cbLandStrBelag_7->setCurrentIndex(3);
            sbLandStrStart_7->setValue(1850);
            sbLandStrEnd_7->setValue(2100);
            cbLandStrBelag_8->setCurrentIndex(3);
            sbLandStrStart_8->setValue(1850);
            sbLandStrEnd_8->setValue(2100);
            cbLandStrBelag_9->setCurrentIndex(3);
            sbLandStrStart_9->setValue(1850);
            sbLandStrEnd_9->setValue(2100);
            cbLandStrBelag_10->setCurrentIndex(3);
            sbLandStrStart_10->setValue(1850);
            sbLandStrEnd_10->setValue(2100);
            cbStraightWays->setCurrentIndex(0);
        };
        break;
    case QMessageBox::No:
        msgBox.close();
        break;
    default:
        break;
    }
    if (rbEinfach->isChecked() == true) {
        gbErweitert->setVisible(false);
    };

    if (rbErweitert->isChecked() == true) {
        gbEinfach->setVisible(false);
    };

    if (rbStrAlt->isChecked() == true) {
        gbStrNeu->setVisible(false);
        gbStrAlt->setVisible(true);
    };

    if (rbStrNeu->isChecked() == true) {
        gbStrNeu->setVisible(true);
        gbStrAlt->setVisible(false);
    };
}

void ConfigureClass::loadValues()
{
    // Lese Simutrans Version ein.
    // Read the simutrans version.
    QFile fileVersion(APP_CONFIG_PATH + "/SimutransVersion.lst");
    fileVersion.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream inVersion(&fileVersion);
    contentVersion = inVersion.readLine();

    /* Load values for the pakset combobox. */
    readSettings();
    cbPakSets->clear();
    // Lese installierte Paksets in die Combobox ein.
    // Fill ComboBox with the installed paksets
    QDir dir(gamePath + "/", "pak*");
    QStringList filter;
    filter << "pak*";

    QStringList dirNames = dir.entryList();

    for (int i = 0; i < (int)dirNames.size(); ++i) {
        cbPakSets->addItem(QPixmap(":/images/images/pak-file.png"), dirNames[i]);
    };
    // Liste installierte Paksets auf.
    // List all installed paksets
    for (int i = 0; i < (int)dirNames.size(); ++i) {
        contentPaksets += dirNames[i] + " || ";
    };
    txtVersion->setText(contentVersion);
    txtInstPakSets->setText("|| " + contentPaksets);

}

void ConfigureClass::loadRoadTypes() {

    /*
     * Pakset: pak128
     * Read the road types to the available comboboxes.
     */
    if (cbPakSets->currentText() == "pak128") {

        QStringList roadTypesCity = (QStringList()
                                 << "Road_030"
                                 << "Road_040"
                                 << "Road_050"
                                 << "Road_070"
                                 << "Road_090");

        cbStadtStrBelag->clear();
        cbStadtStrBelag->addItems(roadTypesCity);
        cbStadtStrBelag_1->clear();
        cbStadtStrBelag_1->addItems(roadTypesCity);
        cbStadtStrBelag_2->clear();
        cbStadtStrBelag_2->addItems(roadTypesCity);
        cbStadtStrBelag_3->clear();
        cbStadtStrBelag_3->addItems(roadTypesCity);
        cbStadtStrBelag_4->clear();
        cbStadtStrBelag_4->addItems(roadTypesCity);
        cbStadtStrBelag_5->clear();
        cbStadtStrBelag_5->addItems(roadTypesCity);
        cbStadtStrBelag_6->clear();
        cbStadtStrBelag_6->addItems(roadTypesCity);
        cbStadtStrBelag_7->clear();
        cbStadtStrBelag_7->addItems(roadTypesCity);
        cbStadtStrBelag_8->clear();
        cbStadtStrBelag_8->addItems(roadTypesCity);
        cbStadtStrBelag_9->clear();
        cbStadtStrBelag_9->addItems(roadTypesCity);
        cbStadtStrBelag_10->clear();
        cbStadtStrBelag_10->addItems(roadTypesCity);

        QStringList roadTypes = (QStringList()
                                 << "Road_050"
                                 << "Road_070"
                                 << "Road_090"
                                 << "Highway_110"
                                 << "Highway_130"
                                 << "Highway_200");

        cbLandStrBelag->clear();
        cbLandStrBelag->addItems(roadTypes);
        cbLandStrBelag_1->clear();
        cbLandStrBelag_1->addItems(roadTypes);
        cbLandStrBelag_2->clear();
        cbLandStrBelag_2->addItems(roadTypes);
        cbLandStrBelag_3->clear();
        cbLandStrBelag_3->addItems(roadTypes);
        cbLandStrBelag_4->clear();
        cbLandStrBelag_4->addItems(roadTypes);
        cbLandStrBelag_5->clear();
        cbLandStrBelag_5->addItems(roadTypes);
        cbLandStrBelag_6->clear();
        cbLandStrBelag_6->addItems(roadTypes);
        cbLandStrBelag_7->clear();
        cbLandStrBelag_7->addItems(roadTypes);
        cbLandStrBelag_8->clear();
        cbLandStrBelag_8->addItems(roadTypes);
        cbLandStrBelag_9->clear();
        cbLandStrBelag_9->addItems(roadTypes);
        cbLandStrBelag_10->clear();
        cbLandStrBelag_10->addItems(roadTypes);
    }
    /*
     * Pakset: PAK128.german
     * Read the road types to the available comboboxes.
     */
    if (cbPakSets->currentText() == "PAK128.german") {

        QStringList roadTypesCity = (QStringList()
                                     << "dirt_cityroad"
                                     << "cobblestone_road20"
                                     << "cobblestone_road40"
                                     << "city_road"
                                     << "city_road50");
        cbStadtStrBelag->clear();
        cbStadtStrBelag->addItems(roadTypesCity);
        cbStadtStrBelag_1->clear();
        cbStadtStrBelag_1->addItems(roadTypesCity);
        cbStadtStrBelag_2->clear();
        cbStadtStrBelag_2->addItems(roadTypesCity);
        cbStadtStrBelag_3->clear();
        cbStadtStrBelag_3->addItems(roadTypesCity);
        cbStadtStrBelag_4->clear();
        cbStadtStrBelag_4->addItems(roadTypesCity);
        cbStadtStrBelag_5->clear();
        cbStadtStrBelag_5->addItems(roadTypesCity);
        cbStadtStrBelag_6->clear();
        cbStadtStrBelag_6->addItems(roadTypesCity);
        cbStadtStrBelag_7->clear();
        cbStadtStrBelag_7->addItems(roadTypesCity);
        cbStadtStrBelag_8->clear();
        cbStadtStrBelag_8->addItems(roadTypesCity);
        cbStadtStrBelag_9->clear();
        cbStadtStrBelag_9->addItems(roadTypesCity);
        cbStadtStrBelag_10->clear();
        cbStadtStrBelag_10->addItems(roadTypesCity);

        QStringList roadTypes = (QStringList()
                                 << "dirt_road"
                                 << "cobblestone_road70"
                                 << "asphalt_road_070"
                                 << "asphalt_road_090"
                                 << "asphalt_road_100"
                                 << "Asphalt_Autobahn"
                                 << "Betonierte Autobahn");

        cbLandStrBelag->clear();
        cbLandStrBelag->addItems(roadTypes);
        cbLandStrBelag_1->clear();
        cbLandStrBelag_1->addItems(roadTypes);
        cbLandStrBelag_2->clear();
        cbLandStrBelag_2->addItems(roadTypes);
        cbLandStrBelag_3->clear();
        cbLandStrBelag_3->addItems(roadTypes);
        cbLandStrBelag_4->clear();
        cbLandStrBelag_4->addItems(roadTypes);
        cbLandStrBelag_5->clear();
        cbLandStrBelag_5->addItems(roadTypes);
        cbLandStrBelag_6->clear();
        cbLandStrBelag_6->addItems(roadTypes);
        cbLandStrBelag_7->clear();
        cbLandStrBelag_7->addItems(roadTypes);
        cbLandStrBelag_8->clear();
        cbLandStrBelag_8->addItems(roadTypes);
        cbLandStrBelag_9->clear();
        cbLandStrBelag_9->addItems(roadTypes);
        cbLandStrBelag_10->clear();
        cbLandStrBelag_10->addItems(roadTypes);
    }

}

void ConfigureClass::createSimuconf()
{
    QDateTime currentTime;
    QString gamePath, datumZeit;

    datumZeit = currentTime.currentDateTime().toLocalTime().toString();

    QSettings simurun_conf(APP_CONFIG_PATH + "/" +
                           APP_NAME + ".conf", QSettings::NativeFormat);
    simurun_conf.beginGroup("Simutrans_Starter");
    gamePath = simurun_conf.value("GamePath").toString();
    simurun_conf.endGroup();

    // Die simuconf.tab schreiben.
    QFile simuconfFile(gamePath + "/" + cbPakSets->currentText()
                       + "/config/simuconf.tab");
    simuconfFile.open(QIODevice::WriteOnly);
    QTextStream simuconfOutput(&simuconfFile);
    simuconfOutput.setCodec("UTF-8");
    simuconfOutput
            << "####################################################################"
            << "\n"
            << "# simuconf.tab \n"
            << "#\n"
            << "# " << tr("Generiert am: ") << datumZeit << "\n"
            << "#\n"
            << "# " << tr("Generiert mit: ")
            << APP_TITLE << " - Version " << APP_VERSION << "\n"
            << "#\n"
            << "# " << tr("Pfad: ") << gamePath << "/" << cbPakSets->currentText()
            << "/config/simuconf.tab\n"
            << "####################################################################"
            << "\n\n";
    if(groupBox_FieldHeight->isChecked()){
        simuconfOutput
                << "###########################  pakset settings  ######################"
                << "\n"
                << "tile_height = " << sB_Height_Z_Direction->value() << "\n"
                << "height_conversion_factor = " << spinBox_height_conversion_factor->value() << "\n\n";
    };

    simuconfOutput
            << "########################## PROGRAM STUFF ###########################"
            << "\n"
            << "singleuser_install = " << cbSingleUser->currentIndex() << "\n"
            << "max_route_steps = " << sbRoutesteps->value() << "\n"
            << "first_beginner = " << cbBeginner->currentIndex() << "\n"
            << "beginner_price_factor = " << spPreisfaktor->value() << "\n"
            << "max_transfers = " << spTransfers->value() << "\n"
            << "max_hops = " << sbSpruenge->value() << "\n"
            << "station_coverage = " << sbAbdeckungen->value() << "\n"
            << "minimum_city_distance = " << sbAbstRath->value() << "\n"
            << "special_building_distance = " << spinBoxAttractionDistance->value() << "\n"
            << "way_straight = 1\n"
            << "way_curve = 2\n"
            << "way_double_curve = 6\n"
            << "way_90_curve = 15\n"
            << "way_slope = 10\n"
            << "way_tunnel = 8\n"
            << "way_max_bridge_len = 15\n"
            << "way_leaving_road = 25\n\n"

            << "############################ DISPLAY STUFF #########################"
            << "\n"
            << "random_pedestrians = " << cbPassanten->currentIndex() << "\n"
            << "only_single_info = " << cbInfo->currentIndex() << "\n"
            << "pedes_and_car_info = " << cbInfoAutos->currentIndex() << "\n"
            << "numbered_stations = " << cbNummeriert->currentIndex() << "\n"
            << "stop_pedestrians = " << cbPassagiereStationen->currentIndex() << "\n"
            << "show_names = " << cbStationsnamen->currentIndex() << "\n"
            << "tree_info = " << cbBaumInfo->currentIndex() << "\n"
            << "townhall_info = " << cbRathausInfo->currentIndex() << "\n"
            << "ground_info = " << cbBodenInfo->currentIndex() << "\n"
            << "drive_left = " << cbLinksfahren->currentIndex() << "\n"
            << "signals_on_left = " << cbLinksfahren->currentIndex() << "\n"
            << "default_citycar_life = " << sbKfzLebensdauer->value() << "\n"
            << "water_animation_ms = " << sbWasser->value() << "\n"
            << "random_wildlife_probability = " << sbTiere->value() << "\n"
            << "random_grounds_probability = " << sbLandschaft->value() << "\n"
            << "citycar_level = " << sbKfzLevel->value() << "\n"
            << "show_vehicle_states = " << cB_Vehicle_Status->currentIndex() << "\n"
            << "visualize_schedule = " << cB_Visualize_Schedule->currentIndex() << "\n"
            << "background_color = " << spinBoxBackgroundColor->value() << "\n"
            << "draw_outside_tile = " << comboBoxDrawOutsideTile->currentIndex() << "\n"
            << "draw_earth_border = " << comboBoxDrawEarthBorder->currentIndex() << "\n"
            << "hide_rail_return_ticket = " << comboBoxInvertedSchedule->currentIndex() << "\n"
            << "show_delete_buttons = " << comboBoxDeleteGame->currentIndex() << "\n"
            << "remove_dummy_player_months = " << spinBoxRemoveDeadPlayer->value() << "\n"
            << "unprotect_abandoned_player_months = " << spinBoxAbandonedPlayer->value() << "\n\n"

            << "############################ WINDOW STUFF ##########################"
            << "\n"
            << "window_buttons_right = " << cbButtons->currentIndex() << "\n"
            << "window_frame_active = " << cbRahmen->currentIndex() << "\n"
            << "show_tooltips = " << cbToolTipps->currentIndex() << "\n"
            << "tooltip_delay = " << sbTooltippsDelay->value() << "\n"
            << "tooltip_duration = " << sbToolTippsDauer->value() << "\n\n"

            << "############################## TIME STUFF ##########################"
            << "\n"
            << "starting_year = " << sbJahr->value() << "\n"
            << "starting_month = " << sbMonate->value() << "\n"
            << "bits_per_month = " << sbForwardSpeed->value() << "\n"
            << "use_timeline = " << cbNeueKfz->currentIndex() << "\n"
            << "show_month = " << cbDatumsFormat->currentIndex() << "\n\n"

            << "############################# SYSTEM STUFF #########################"
            << "\n"
            << "saveformat = " << cbSpeicherFormat->currentText() << "\n"
            << "autosave = " << cbMonatIntervall->currentIndex() << "\n"
            << "add_player_name_to_message = " << cbMessages->currentIndex() << "\n"
            << "frames_per_second = " << sbFPS->value() << "\n"
            << "simple_drawing_tile_size = " << sbSimpleDawTiles->value() << "\n"
            << "fast_forward = " << sbFastForward->value() << "\n"
            << "threads = " << spinBoxThreadsToUse->value() << "\n"
            << "simple_drawing_fast_forward = " << comboBoxDrawingFast->currentIndex() << "\n\n"

            << "########################### NETWORK STUFF ##########################"
            << "\n"
            << "server_frames_ahead = " << sbDelay->value() << "\n"
            << "server_frames_per_step = " << sbUpdates->value() << "\n"
            << "additional_client_frames_behind = " << sbTimeDelay->value() << "\n"
            << "server_frames_between_checks = " << sbClientCheck->value() << "\n"
            << "server_announce = " << cbServerAnnounce->currentIndex() << "\n"
            << "server_announce_intervall = " << sbAboIntervall->value() << "\n"
            << "nickname = " << txtNickName->text() << "\n"
            << "server_name = " << txtServerName->text() << "\n"
            << "server_admin_pw = " << txtAdminPW->text() << "\n"
            << "server_dns = " << txtServerAdresse->text() << "\n"
            << "server_email = " << txtServerEmail->text() << "\n"
            << "server_pakurl = " << txtPakSetUrl->text() << "\n"
            << "server_infurl = " << txtServerInfo->text() << "\n"
            << "server_comment = " << txtKommentar->toPlainText() << "\n"
            << "pause_server_no_clients = " << cbServerPause->currentIndex() << "\n"
            << "chat_transparency = " << spinBoxChatTransparency->value() << "\n\n"

            << "########################### FACTORY STUFF ##########################"
            << "\n"
            << "crossconnect_factories_percentage = " << sbProzent->value() << "\n"
            << "industry_increase_every = " << sbErweitern->value() << "\n"
            << "just_in_time = " << cbBeliefern->currentIndex() << "\n"
            << "maximum_intransit_percentage = " << spinBoxMinAmount->value() << "\n"
            << "electric_promille = " << sbPromille->value() << "\n"
            << "factory_arrival_periods = " << sbPassInterwal->value() << "\n"
            << "min_factory_spacing = " << spinBoxMinFactorySpacing->value() << "\n"
            << "max_factory_spacing = " << spinBoxMaxFactorySpacing->value() << "\n"
            << "max_factory_spacing_percentage = " << spinBoxMaxFactorySpacingPercent->value() << "\n"
            << "factory_enforce_demand = " << cbFabrikAnforderung->currentIndex() << "\n"
            << "allow_underground_transformers = " << comboBoxUndergroundTransformers->currentIndex() << "\n\n"

            << "##################### GOODS UND PASSENGER STUFF ####################"
            << "\n"
            << "passenger_multiplier = " << sbMultiplikatorPassagiere->value() << "\n"
            << "electricity_multiplier = " << sbMultiplikatorEnergie->value() << "\n"
            << "mail_multiplier = " << sbMultiplikatorPost->value() << "\n"
            << "goods_multiplier = " << sbMultiplikatorGoods->value() << "\n"
            << "passenger_factor = " << sbPassagierFoktor->value() << "\n"
            << "no_routing_over_overcrowded = " << cbStationUeberfuellt->currentIndex()
            << "\n"
            << "factory_worker_percentage = " << sbArbeiteraufkommen->value() << "\n"
            << "tourist_percentage = " << sbTouristenaufkommen->value() << "\n"
            << "factory_worker_radius = " << sbRadius->value() << "\n"
            << "factory_worker_minimum_towns = " << sbMinCities->value() << "\n"
            << "avoid_overcrowding = " << cbZielUeberfuellt->currentIndex() << "\n"
            << "pay_for_total_distance = " << cbTransportCash->currentIndex() << "\n"
            << "factory_worker_maximum_towns = " << sbMaxCities->value() << "\n"
            << "growthfactor_villages = " << sbUnter_1000->value() << "\n"
            << "growthfactor_cities = " << sb1000_10000->value() << "\n"
            << "growthfactor_capitals = " << sbUeber_10000->value() << "\n"
            << "city_short_range_percentage = " << sbArbeiterKleinerRadius->value()
            << "\n"
            << "city_medium_range_percentage = " << ArbeiterGrosserRadius->value()
            << "\n"
            << "city_short_range_radius = " << sbStadtRadiusKurz->value() << "\n"
            << "city_medium_range_radius = " << sbStadtRadiusMittel->value() << "\n"
            << "seperate_halt_capacities = " << cbKapazitaeten->currentIndex() << "\n"

            << "locality_factor[0]="  << sb_Jahr_1->value() << ","
            << sb_Jahr_Ziel_1->value() << "\n";

    if (chb_Ziele_2->isChecked() == true){
        simuconfOutput
                << "locality_factor[1]=" << sb_Jahr_2->value() << ","
                << sb_Jahr_Ziel_2->value() <<  "\n";
    };
    if (chb_Ziele_3->isChecked() == true){
        simuconfOutput
                << "locality_factor[2]=" << sb_Jahr_3->value() << ","
                << sb_Jahr_Ziel_3->value() <<  "\n";
    };
    if (chb_Ziele_4->isChecked() == true){
        simuconfOutput
                << "locality_factor[3]=" << sb_Jahr_4->value() << ","
                << sb_Jahr_Ziel_4->value() <<  "\n";
    };
    if (chb_Ziele_5->isChecked() == true){
        simuconfOutput
                << "locality_factor[4]=" << sb_Jahr_5->value() << ","
                << sb_Jahr_Ziel_5->value() <<  "\n";
    };
    if (chb_Ziele_6->isChecked() == true){
        simuconfOutput
                << "locality_factor[5]=" << sb_Jahr_6->value() << ","
                << sb_Jahr_Ziel_6->value() <<  "\n";
    };
    if (chb_Ziele_7->isChecked() == true){
        simuconfOutput
                << "locality_factor[6]=" << sb_Jahr_7->value() << ","
                << sb_Jahr_Ziel_7->value() <<  "\n";
    };
    if (chb_Ziele_8->isChecked() == true){
        simuconfOutput
                << "locality_factor[7]=" << sb_Jahr_8->value() << ","
                << sb_Jahr_Ziel_8->value() <<  "\n";
    };
    if (chb_Ziele_9->isChecked() == true){
        simuconfOutput
                << "locality_factor[8]=" << sb_Jahr_9->value() << ","
                << sb_Jahr_Ziel_9->value() <<  "\n";
    };
    if (chb_Ziele_10->isChecked() == true){
        simuconfOutput
                << "locality_factor[9]=" << sb_Jahr_10->value() << ","
                << sb_Jahr_Ziel_10->value() <<  "\n";
    };

    simuconfOutput
            << "\n"
            << "########################## MONEY STUFF #############################"
            << "\n"
            << "left_to_right_graphs = " << cbStatistik->currentIndex() << "\n"
            << "allow_buying_obsolete_vehicles = " << cbAusgemustert->currentIndex()
            << "\n"
            << "used_vehicle_reduction = " << sbWertverlustKfz->value() << "0" << "\n"
            << "toll_runningcost_percentage = " << sb_Maut->value() << "\n"
            << "toll_waycost_percentage = " << sb_StrKosten->value() << "\n\n";

    if (rbEinfach->isChecked() == true){
        simuconfOutput
                << "# Starting money old.\n"
                << "starting_money = " << sbStartkapital->value() << "00" << "\n";
    };

    if (rbErweitert->isChecked() == true){
        simuconfOutput
                << "# Starting money new.\n"
                << "starting_money[0]=" << sbStartJahr_1->value() << ","
                << sbStartkapital_1->value() << "00" << ","
                << cbInterpoliert_1->currentIndex()
                << "\n";

        if (chb_2->isChecked() == true){
            simuconfOutput
                    << "starting_money[1]=" << sbStartJahr_2->value() << ","
                    << sbStartkapital_2->value() << "00" << ","
                    << cbInterpoliert_2->currentIndex()
                    << "\n";
        };
        if (chb_3->isChecked() == true){
            simuconfOutput
                    << "starting_money[2]=" << sbStartJahr_3->value() << ","
                    << sbStartkapital_3->value() << "00" << ","
                    << cbInterpoliert_3->currentIndex()
                    << "\n";
        };
        if (chb_4->isChecked() == true){
            simuconfOutput
                    << "starting_money[3]=" << sbStartJahr_4->value() << ","
                    << sbStartkapital_4->value() << "00" << ","
                    << cbInterpoliert_4->currentIndex()
                    << "\n";
        };
        if (chb_5->isChecked() == true){
            simuconfOutput
                    << "starting_money[4]=" << sbStartJahr_5->value() << ","
                    << sbStartkapital_5->value() << "00" << ","
                    << cbInterpoliert_5->currentIndex()
                    << "\n";
        };
        if (chb_6->isChecked() == true){
            simuconfOutput
                    << "starting_money[5]=" << sbStartJahr_6->value() << ","
                    << sbStartkapital_6->value() << "00" << ","
                    << cbInterpoliert_6->currentIndex()
                    << "\n";
        };
        if (chb_7->isChecked() == true){
            simuconfOutput
                    << "starting_money[6]=" << sbStartJahr_7->value() << ","
                    << sbStartkapital_7->value() << "00" << ","
                    << cbInterpoliert_7->currentIndex()
                    << "\n";
        };
        if (chb_8->isChecked() == true){
            simuconfOutput
                    << "starting_money[7]=" << sbStartJahr_8->value() << ","
                    << sbStartkapital_8->value() << "00" << ","
                    << cbInterpoliert_8->currentIndex()
                    << "\n";
        };
        if (chb_9->isChecked() == true){
            simuconfOutput
                    << "starting_money[8]=" << sbStartJahr_9->value() << ","
                    << sbStartkapital_9->value() << "00" << ","
                    << cbInterpoliert_9->currentIndex()
                    << "\n";
        };
        if (chb_10->isChecked() == true){
            simuconfOutput
                    << "starting_money[9]=" << sbStartJahr_10->value() << ","
                    << sbStartkapital_10->value() << "00" << ","
                    << cbInterpoliert_10->currentIndex()
                    << "\n";
        };
    };
    simuconfOutput
            << "\n# Maintenance costs.\n"
            << "maintenance_building = " << sbGebaeude->value() << "\n"
            << "cost_multiply_dock = " << sbSchiffsdock->value() << "\n"
            << "cost_multiply_station = " << sbBahnhof->value() << "\n"
            << "cost_multiply_roadstop = " << sbHaltestelle->value() << "\n"
            << "cost_multiply_airterminal = " << sbTerminal->value() << "\n"
            << "cost_multiply_post = " << sbPosthalt->value() << "\n"
            << "cost_multiply_headquarter = " << sbHauptquartier->value() << "\n"
            << "cost_depot_air = " << sbHanger->value() << "\n"
            << "cost_depot_rail = " << sbLockschuppen->value() << "\n"
            << "cost_depot_road = " << sbKfzdepot->value() << "\n"
            << "cost_depot_ship = " << sbSchiffsdepot->value() << "\n"
            << "cost_signal = " << sbSignale->value() << "\n"
            << "cost_tunnel = " << sbTunnel->value() << "\n"
            << "cost_third_rail = " << sbDrittesGleis->value() << "\n"
            << "cost_buy_land = " << sbLanderweiterung->value() << "\n"
            << "cost_alter_land = " << sbLandaenderung->value() << "\n"
            << "cost_set_slope = " << sbSlope->value() << "\n"
            << "cost_found_city = " << sbNeueStadt->value() << "\n"
            << "cost_multiply_found_industry = " << sbIndustrie->value() << "\n"
            << "cost_remove_tree = " << sbBaumFaellen->value() << "\n"
            << "cost_multiply_remove_haus = " << sbAbreissen->value() << "\n"
            << "cost_multiply_remove_field = " << sbFeldEntfernen->value() << "\n"
            << "cost_transformer = " << sbTrafoKaufen->value() << "\n"
            << "cost_maintain_transformer = " << sbTrafoWartung->value() << "\n"
            << "bonus_basefactor = " << sb_Bonus_Basefactor->value() << "\n\n";

    simuconfOutput
            << "##################### ROAD, FORREST, RIVER STUFF #####################"
            << "\n"
            << "intercity_road_length=" << sbLandStrLaenge->value() << "\n"
            << "straight_way_without_control=" << cbStraightWays->currentIndex() << "\n\n";

    if (rbStrAlt->isChecked() == true){
        simuconfOutput
                << "# Road settings old.\n"
                << "city_road_type=" << cbStadtStrBelag->currentText() << "\n"
                << "intercity_road_type=" << cbLandStrBelag->currentText() << "\n";
        simuconfOutput << "\n";
    };

    if (rbStrNeu->isChecked() == true){
        simuconfOutput
                << "# Road settings new.\n"
                << "city_road[0]=" << cbStadtStrBelag_1->currentText() << ","
                << sbStadtStrStart_1->value() << "," << sbStadtStrEnd_1->value() <<"\n";

        if (chbStadtStr_2->isChecked() == true) {
            simuconfOutput
                    << "city_road[1]=" << cbStadtStrBelag_2->currentText() << ","
                    << sbStadtStrStart_2->value() << "," << sbStadtStrEnd_2->value() <<"\n";
        };
        if (chbStadtStr_3->isChecked() == true) {
            simuconfOutput
                    << "city_road[2]=" << cbStadtStrBelag_3->currentText() << ","
                    << sbStadtStrStart_3->value() << "," << sbStadtStrEnd_3->value() <<"\n";
        };
        if (chbStadtStr_4->isChecked() == true) {
            simuconfOutput
                    << "city_road[3]=" << cbStadtStrBelag_4->currentText() << ","
                    << sbStadtStrStart_4->value() << "," << sbStadtStrEnd_4->value() <<"\n";
        };
        if (chbStadtStr_5->isChecked() == true) {
            simuconfOutput
                    << "city_road[4]=" << cbStadtStrBelag_5->currentText() << ","
                    << sbStadtStrStart_5->value() << "," << sbStadtStrEnd_5->value() <<"\n";
        };
        if (chbStadtStr_6->isChecked() == true) {
            simuconfOutput
                    << "city_road[5]=" << cbStadtStrBelag_6->currentText() << ","
                    << sbStadtStrStart_6->value() << "," << sbStadtStrEnd_6->value() <<"\n";
        };
        if (chbStadtStr_7->isChecked() == true) {
            simuconfOutput
                    << "city_road[6]=" << cbStadtStrBelag_7->currentText() << ","
                    << sbStadtStrStart_7->value() << "," << sbStadtStrEnd_7->value() <<"\n";
        };
        if (chbStadtStr_8->isChecked() == true) {
            simuconfOutput
                    << "city_road[7]=" << cbStadtStrBelag_8->currentText() << ","
                    << sbStadtStrStart_8->value() << "," << sbStadtStrEnd_8->value() <<"\n";
        };
        if (chbStadtStr_9->isChecked() == true) {
            simuconfOutput
                    << "city_road[8]=" << cbStadtStrBelag_9->currentText() << ","
                    << sbStadtStrStart_9->value() << "," << sbStadtStrEnd_9->value() <<"\n";
        };
        if (chbStadtStr_10->isChecked() == true) {
            simuconfOutput
                    << "city_road[9]=" << cbStadtStrBelag_10->currentText() << ","
                    << sbStadtStrStart_10->value() << "," << sbStadtStrEnd_10->value() <<"\n";
        };


        simuconfOutput
                << "intercity_road[0]=" << cbLandStrBelag_1->currentText() << ","
                << sbLandStrStart_1->value() << "," << sbLandStrEnd_1->value() <<"\n";

        if (chbLandStr_2->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[1]=" << cbLandStrBelag_2->currentText() << ","
                    << sbLandStrStart_2->value() << "," << sbLandStrEnd_2->value() <<"\n";
        };
        if (chbLandStr_3->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[2]=" << cbLandStrBelag_3->currentText() << ","
                    << sbLandStrStart_3->value() << "," << sbLandStrEnd_3->value() <<"\n";
        };
        if (chbLandStr_4->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[3]=" << cbLandStrBelag_4->currentText() << ","
                    << sbLandStrStart_4->value() << "," << sbLandStrEnd_4->value() <<"\n";
        };
        if (chbLandStr_5->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[4]=" << cbLandStrBelag_5->currentText() << ","
                    << sbLandStrStart_5->value() << "," << sbLandStrEnd_5->value() <<"\n";
        };
        if (chbLandStr_6->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[5]=" << cbLandStrBelag_6->currentText() << ","
                    << sbLandStrStart_6->value() << "," << sbLandStrEnd_6->value() <<"\n";
        };
        if (chbLandStr_7->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[6]=" << cbLandStrBelag_7->currentText() << ","
                    << sbLandStrStart_7->value() << "," << sbLandStrEnd_7->value() <<"\n";
        };
        if (chbLandStr_8->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[7]=" << cbLandStrBelag_8->currentText() << ","
                    << sbLandStrStart_8->value() << "," << sbLandStrEnd_8->value() <<"\n";
        };
        if (chbLandStr_9->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[8]=" << cbLandStrBelag_9->currentText() << ","
                    << sbLandStrStart_9->value() << "," << sbLandStrEnd_9->value() <<"\n";
        };
        if (chbLandStr_10->isChecked() == true) {
            simuconfOutput
                    << "intercity_road[9]=" << cbLandStrBelag_10->currentText() << ","
                    << sbLandStrStart_10->value() << "," << sbLandStrEnd_10->value() <<"\n";
        };
        simuconfOutput << "\n";
    };
    simuconfOutput
            << "# Forrest settings pak128.\n"
            << "no_tree = " << cbTreesShow->currentIndex() << "\n"
            << "tree_climates = " << cbTreesKlima->currentIndex() << "\n"
            << "no_tree_climates = " << sbNoTreesKlima->value() << "\n"
            << "max_no_of_trees_on_square = " << sbBaumFeld->value() << "\n"
            << "forest_base_size = " << sbForrestSize->value() << "\n"
            << "forest_map_size_divisor = " << sbMapDivisor->value() << "\n"
            << "forest_count_divisor = " << sbForrestDivisor->value() << "\n"
            << "forest_inverse_spare_tree_density = " << sbTreesDichte->value() << "\n\n";
    if (cbPakSets->currentText() == "pak128") {
        simuconfOutput
                    << "# River settings.\n"
                    << "river_type[0]=river_30" <<"\n"
                    << "river_type[1]=river_20" <<"\n"
                    << "river_type[2]=river_10" <<"\n"
                    << "river_type[3]=river_0b" <<"\n"
                    << "river_type[4]=river_0a" <<"\n";
    };
    if (cbPakSets->currentText() == "PAK128.german") {
        simuconfOutput
                    << "# River settings PAK128.german.\n"
                    << "river_type[0]=river_00" <<"\n"
                    << "river_type[1]=river_10" <<"\n"
                    << "river_type[2]=river_20" <<"\n"
                    << "river_type[3]=river_30" <<"\n"
                    << "river_type[4]=river_40" <<"\n";
    };
    simuconfOutput
            << "river_number = " << sbFlussMenge->value() << "\n"
            << "river_min_length = " << sbFlussMinLang->value() << "\n"
            << "river_max_length = " << sbFlussMaxLang->value() << "\n";
    // << "=" <<  << "\n"

}

void ConfigureClass::on_sbAbdeckungen_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbAbdeckungen->setSuffix(tr(" Felder"));
    } else if (wert == 1) {
        sbAbdeckungen->setSuffix(tr(" Feld"));
    } else {
        sbAbdeckungen->setSuffix(tr(" Felder"));
    };
}

void ConfigureClass::on_sbKfzLebensdauer_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbKfzLebensdauer->setSuffix(tr(" Monate"));
    } else if (wert == 1) {
        sbKfzLebensdauer->setSuffix(tr(" Monat"));
    } else {
        sbKfzLebensdauer->setSuffix(tr(" Monate"));
    };
}

void ConfigureClass::on_sbKfzLevel_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbKfzLevel->setSuffix(tr(" Typen"));
    } else if (wert == 1) {
        sbKfzLevel->setSuffix(tr(" Typ"));
    } else {
        sbKfzLevel->setSuffix(tr(" Typen"));
    };
}

void ConfigureClass::on_spinBoxMinFactorySpacing_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        spinBoxMinFactorySpacing->setSuffix(tr(" Felder"));
    } else if (wert == 1) {
        spinBoxMinFactorySpacing->setSuffix(tr(" Feld"));
    } else {
        spinBoxMinFactorySpacing->setSuffix(tr(" Felder"));
    };
}


void ConfigureClass::on_sbLandStrLaenge_valueChanged(int arg1)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (arg1 == 0) {
        sbLandStrLaenge->setSuffix(tr(" Felder"));
    } else if (arg1 == 1) {
        sbLandStrLaenge->setSuffix(tr(" Feld"));
    } else {
        sbLandStrLaenge->setSuffix(tr(" Felder"));
    };
}

void ConfigureClass::on_sbRadius_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbRadius->setSuffix(tr(" Felder"));
    } else if (wert == 1) {
        sbRadius->setSuffix(tr(" Feld"));
    } else {
        sbRadius->setSuffix(tr(" Felder"));
    };
}


void ConfigureClass::on_sbAbstRath_valueChanged(int arg1)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (arg1 == 0) {
        sbAbstRath->setSuffix(tr(" Felder"));
    } else if (arg1 == 1) {
        sbAbstRath->setSuffix(tr(" Feld"));
    } else {
        sbAbstRath->setSuffix(tr(" Felder"));
    };
}

void ConfigureClass::on_sbMinCities_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbMinCities->setSuffix(tr(" Städte"));
    } else if (wert == 1) {
        sbMinCities->setSuffix(tr(" Stadt"));
    } else {
        sbMinCities->setSuffix(tr(" Städte"));
    };
}

void ConfigureClass::on_sbMaxCities_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbMaxCities->setSuffix(tr(" Frames"));
    } else if (wert == 1) {
        sbMaxCities->setSuffix(tr(" Stadt"));
    } else {
        sbMaxCities->setSuffix(tr(" Städte"));
    };
}

void ConfigureClass::on_sbDelay_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbDelay->setSuffix(tr(" Frames"));
    } else if (wert == 1) {
        sbDelay->setSuffix(tr(" Frame"));
    } else {
        sbDelay->setSuffix(tr(" Frames"));
    };
}

void ConfigureClass::on_sbUpdates_valueChanged(int wert)
{
    // Suffixe für Einzahl und Mehrzahl.
    // Suffixes for singular and plural.
    if (wert == 0) {
        sbUpdates->setSuffix(tr(" Frames"));
    } else if (wert == 1) {
        sbUpdates->setSuffix(tr(" Frame"));
    } else {
        sbUpdates->setSuffix(tr(" Frames"));
    };
}

void ConfigureClass::createLanguageMenu() {

    QDir dir(LANG_PATH);
    QStringList filter;
    filter << QString(APP_NAME) + "_*.qm";
    QStringList fileNames = dir.entryList(filter);

    QString languageName;
    QIcon * countryFlag;
    LanguageInfo info;
    //info = languages->getLanguageInfo("simurun_en.qm");

    languageName = info.niceName;
    QString trlangName = tr(languageName.toStdString().c_str());

    countryFlag = new QIcon(FLAGS_PATH + info.flagFile);

    cbSprachen->setInsertPolicy(QComboBox::InsertAtTop);
    cbSprachen->insertItem(0, *(countryFlag), trlangName);

    //fileToNiceName->insert(trlangName, "simurun_en.qm");

    for (int i = 0; i < (int)fileNames.size(); ++i) {
        info = languages->getLanguageInfo(fileNames[i]);
        languageName = info.niceName;

        trlangName = tr(languageName.toStdString().c_str());
        countryFlag = new QIcon(FLAGS_PATH +
                                info.flagFile);

        cbSprachen->insertItem(i+1, *(countryFlag), trlangName);

        fileToNiceName->insert(trlangName, fileNames[i]);
    };
}

void ConfigureClass::on_LanguageAppGroup_clicked(bool checked)
{
    if (checked == false) {
        cbSprachen->setCurrentIndex(0);
    };
}

void ConfigureClass::on_tabMain_currentChanged(int index)
{
    setWindowTitle(tr("%1 %2 - %3 %4")
                   .arg(APP_TITLE)
                   .arg(APP_VERSION)
                   .arg(tr("Konfiguration"))
                   .arg("[ " + tabMain->tabText(index) + " ]"));
    if (index == 1){
        pbGenerateConfig->setEnabled(true);
        setWindowTitle(tr("%1 %2 - %3 %4 - %5")
                       .arg(APP_TITLE)
                       .arg(APP_VERSION)
                       .arg(tr("Konfiguration"))
                       .arg("[ " + tabMain->tabText(index))
                       .arg(tabSettings->tabText(tabSettings->currentIndex()) + " ]"));
    } else {
        pbGenerateConfig->setDisabled(true);
    }
}

void ConfigureClass::on_tabSettings_currentChanged(int index)
{
    if (tabMain->currentIndex() == 1){
        setWindowTitle(tr("%1 %2 - %3 %4 - %5")
                       .arg(APP_TITLE)
                       .arg(APP_VERSION)
                       .arg(tr("Konfiguration"))
                       .arg("[ " + tabMain->tabText(tabMain->currentIndex()))
                       .arg(tabSettings->tabText(index) + " ]"));
    }
}

void ConfigureClass::executeChanges()
{
    QProcess copyCityFile, copyDesktopFile;
    QDir desktopFile;

    // Ausgewählte Städtenamen Datei nach citylist_de.txt umbenennen.
    // Rename selected citylist_xx_de.txt to citylist_de.txt.
    if (cbStaedtenamen->currentIndex() == 0) {
        copyCityFile.execute("cp -vf " + QString(APPS_TPL_PATH) +
                             "citylist_de_de.txt "
                             + gamePath + "/" + paksetActive
                             + "/text/citylist_de.txt");

    } else if (cbStaedtenamen->currentIndex() == 1) {
        copyCityFile.execute("cp -vf " + QString(APPS_TPL_PATH) +
                             "citylist_de_at.txt "
                             + gamePath + "/" + paksetActive
                             + "/text/citylist_de.txt");
    } else if (cbStaedtenamen->currentIndex() == 2) {
        copyCityFile.execute("cp -vf " + QString(APPS_TPL_PATH) +
                             "citylist_ch.txt "
                             + gamePath + "/" + paksetActive
                             + "/text/citylist_de.txt");
    };

    // Kopiere oder entferne das Desktop Icon je nach CheckBox Wert.
    // Copy or remove the desktop icon, depending on the CheckBox value.
    if (cbIconDesktop->isChecked() == true) {
        if (desktopFile.exists(desktopFile.homePath() + "/Desktop/") == true) {
            copyDesktopFile.execute("cp -vf " + QString(DESKTOP_FILE) +
                                    "simurun2.desktop "
                                    + desktopFile.homePath() + "/Desktop/");
        };
    } else {
        copyDesktopFile.execute("rm -vf " + desktopFile.homePath() +
                                "/Desktop/" + APP_NAME + ".desktop");
    };
}

void ConfigureClass::on_pbSchliessen_clicked()
{
    close();
}

void ConfigureClass::on_pbSpeichern_clicked()
{
    writeSettings();
    writeSimutransSettings();
    executeChanges();
}

void ConfigureClass::on_cbBeginner_currentIndexChanged(int index)
{
    if (index == 0) {
        spPreisfaktor->setEnabled(false);
    } else {
        spPreisfaktor->setEnabled(true);
    };
}

void ConfigureClass::on_cbPakSets_currentIndexChanged(QString)
{

    loadRoadTypes();
    // readSettings();
    readSimutransSettings();

}

void ConfigureClass::on_pbDafault_clicked()
{
    loadDefaultValues();
}

void ConfigureClass::on_chb_2_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_2->setEnabled(true);
        sbStartJahr_2->setEnabled(true);
        cbInterpoliert_2->setEnabled(true);
        chb_3->setEnabled(true);
    } else {
        sbStartkapital_2->setEnabled(false);
        sbStartJahr_2->setEnabled(false);
        cbInterpoliert_2->setEnabled(false);
        chb_3->setEnabled(false);
        chb_3->setChecked(false);
    };
}

void ConfigureClass::on_chb_3_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_3->setEnabled(true);
        sbStartJahr_3->setEnabled(true);
        cbInterpoliert_3->setEnabled(true);
        chb_4->setEnabled(true);
    } else {
        sbStartkapital_3->setEnabled(false);
        sbStartJahr_3->setEnabled(false);
        cbInterpoliert_3->setEnabled(false);
        chb_4->setEnabled(false);
        chb_4->setChecked(false);
    };
}

void ConfigureClass::on_chb_4_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_4->setEnabled(true);
        sbStartJahr_4->setEnabled(true);
        cbInterpoliert_4->setEnabled(true);
        chb_5->setEnabled(true);
    } else {
        sbStartkapital_4->setEnabled(false);
        sbStartJahr_4->setEnabled(false);
        cbInterpoliert_4->setEnabled(false);
        chb_5->setEnabled(false);
        chb_5->setChecked(false);
    };
}

void ConfigureClass::on_chb_5_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_5->setEnabled(true);
        sbStartJahr_5->setEnabled(true);
        cbInterpoliert_5->setEnabled(true);
        chb_6->setEnabled(true);
    } else {
        sbStartkapital_5->setEnabled(false);
        sbStartJahr_5->setEnabled(false);
        cbInterpoliert_5->setEnabled(false);
        chb_6->setEnabled(false);
        chb_6->setChecked(false);
    };
}

void ConfigureClass::on_chb_6_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_6->setEnabled(true);
        sbStartJahr_6->setEnabled(true);
        cbInterpoliert_6->setEnabled(true);
        chb_7->setEnabled(true);
    } else {
        sbStartkapital_6->setEnabled(false);
        sbStartJahr_6->setEnabled(false);
        cbInterpoliert_6->setEnabled(false);
        chb_7->setEnabled(false);
        chb_7->setChecked(false);
    };
}

void ConfigureClass::on_chb_7_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_7->setEnabled(true);
        sbStartJahr_7->setEnabled(true);
        cbInterpoliert_7->setEnabled(true);
        chb_8->setEnabled(true);
    } else {
        sbStartkapital_7->setEnabled(false);
        sbStartJahr_7->setEnabled(false);
        cbInterpoliert_7->setEnabled(false);
        chb_8->setEnabled(false);
        chb_8->setChecked(false);
    };
}

void ConfigureClass::on_chb_8_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_8->setEnabled(true);
        sbStartJahr_8->setEnabled(true);
        cbInterpoliert_8->setEnabled(true);
        chb_9->setEnabled(true);
    } else {
        sbStartkapital_8->setEnabled(false);
        sbStartJahr_8->setEnabled(false);
        cbInterpoliert_8->setEnabled(false);
        chb_9->setEnabled(false);
        chb_9->setChecked(false);
    };
}

void ConfigureClass::on_chb_9_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_9->setEnabled(true);
        sbStartJahr_9->setEnabled(true);
        cbInterpoliert_9->setEnabled(true);
        chb_10->setEnabled(true);
    } else {
        sbStartkapital_9->setEnabled(false);
        sbStartJahr_9->setEnabled(false);
        cbInterpoliert_9->setEnabled(false);
        chb_10->setEnabled(false);
        chb_10->setChecked(false);
    };
}

void ConfigureClass::on_chb_10_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sbStartkapital_10->setEnabled(true);
        sbStartJahr_10->setEnabled(true);
        cbInterpoliert_10->setEnabled(true);
    } else {
        sbStartkapital_10->setEnabled(false);
        sbStartJahr_10->setEnabled(false);
        cbInterpoliert_10->setEnabled(false);
    };
}

void ConfigureClass::on_chbStadtStr_2_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_2->setEnabled(true);
        sbStadtStrStart_2->setEnabled(true);
        sbStadtStrEnd_2->setEnabled(true);
        chbStadtStr_3->setEnabled(true);
    } else {
        cbStadtStrBelag_2->setEnabled(false);
        sbStadtStrStart_2->setEnabled(false);
        sbStadtStrEnd_2->setEnabled(false);
        chbStadtStr_3->setEnabled(false);
        chbStadtStr_3->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_3_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_3->setEnabled(true);
        sbStadtStrStart_3->setEnabled(true);
        sbStadtStrEnd_3->setEnabled(true);
        chbStadtStr_4->setEnabled(true);
    } else {
        cbStadtStrBelag_3->setEnabled(false);
        sbStadtStrStart_3->setEnabled(false);
        sbStadtStrEnd_3->setEnabled(false);
        chbStadtStr_4->setEnabled(false);
        chbStadtStr_4->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_4_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_4->setEnabled(true);
        sbStadtStrStart_4->setEnabled(true);
        sbStadtStrEnd_4->setEnabled(true);
        chbStadtStr_5->setEnabled(true);
    } else {
        cbStadtStrBelag_4->setEnabled(false);
        sbStadtStrStart_4->setEnabled(false);
        sbStadtStrEnd_4->setEnabled(false);
        chbStadtStr_5->setEnabled(false);
        chbStadtStr_5->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_5_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_5->setEnabled(true);
        sbStadtStrStart_5->setEnabled(true);
        sbStadtStrEnd_5->setEnabled(true);
        chbStadtStr_6->setEnabled(true);
    } else {
        cbStadtStrBelag_5->setEnabled(false);
        sbStadtStrStart_5->setEnabled(false);
        sbStadtStrEnd_5->setEnabled(false);
        chbStadtStr_6->setEnabled(false);
        chbStadtStr_6->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_6_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_6->setEnabled(true);
        sbStadtStrStart_6->setEnabled(true);
        sbStadtStrEnd_6->setEnabled(true);
        chbStadtStr_7->setEnabled(true);
    } else {
        cbStadtStrBelag_6->setEnabled(false);
        sbStadtStrStart_6->setEnabled(false);
        sbStadtStrEnd_6->setEnabled(false);
        chbStadtStr_7->setEnabled(false);
        chbStadtStr_7->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_7_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_7->setEnabled(true);
        sbStadtStrStart_7->setEnabled(true);
        sbStadtStrEnd_7->setEnabled(true);
        chbStadtStr_8->setEnabled(true);
    } else {
        cbStadtStrBelag_7->setEnabled(false);
        sbStadtStrStart_7->setEnabled(false);
        sbStadtStrEnd_7->setEnabled(false);
        chbStadtStr_8->setEnabled(false);
        chbStadtStr_8->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_8_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_8->setEnabled(true);
        sbStadtStrStart_8->setEnabled(true);
        sbStadtStrEnd_8->setEnabled(true);
        chbStadtStr_9->setEnabled(true);
    } else {
        cbStadtStrBelag_8->setEnabled(false);
        sbStadtStrStart_8->setEnabled(false);
        sbStadtStrEnd_8->setEnabled(false);
        chbStadtStr_9->setEnabled(false);
        chbStadtStr_9->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_9_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_9->setEnabled(true);
        sbStadtStrStart_9->setEnabled(true);
        sbStadtStrEnd_9->setEnabled(true);
        chbStadtStr_10->setEnabled(true);
    } else {
        cbStadtStrBelag_9->setEnabled(false);
        sbStadtStrStart_9->setEnabled(false);
        sbStadtStrEnd_9->setEnabled(false);
        chbStadtStr_10->setEnabled(false);
        chbStadtStr_10->setChecked(false);
    };
}

void ConfigureClass::on_chbStadtStr_10_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbStadtStrBelag_10->setEnabled(true);
        sbStadtStrStart_10->setEnabled(true);
        sbStadtStrEnd_10->setEnabled(true);
    } else {
        cbStadtStrBelag_10->setEnabled(false);
        sbStadtStrStart_10->setEnabled(false);
        sbStadtStrEnd_10->setEnabled(false);
    };
}

void ConfigureClass::on_chbLandStr_2_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_2->setEnabled(true);
        sbLandStrStart_2->setEnabled(true);
        sbLandStrEnd_2->setEnabled(true);
        chbLandStr_3->setEnabled(true);
    } else {
        cbLandStrBelag_2->setEnabled(false);
        sbLandStrStart_2->setEnabled(false);
        sbLandStrEnd_2->setEnabled(false);
        chbLandStr_3->setEnabled(false);
        chbLandStr_3->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_3_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_3->setEnabled(true);
        sbLandStrStart_3->setEnabled(true);
        sbLandStrEnd_3->setEnabled(true);
        chbLandStr_4->setEnabled(true);
    } else {
        cbLandStrBelag_3->setEnabled(false);
        sbLandStrStart_3->setEnabled(false);
        sbLandStrEnd_3->setEnabled(false);
        chbLandStr_4->setEnabled(false);
        chbLandStr_4->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_4_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_4->setEnabled(true);
        sbLandStrStart_4->setEnabled(true);
        sbLandStrEnd_4->setEnabled(true);
        chbLandStr_5->setEnabled(true);
    } else {
        cbLandStrBelag_4->setEnabled(false);
        sbLandStrStart_4->setEnabled(false);
        sbLandStrEnd_4->setEnabled(false);
        chbLandStr_5->setEnabled(false);
        chbLandStr_5->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_5_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_5->setEnabled(true);
        sbLandStrStart_5->setEnabled(true);
        sbLandStrEnd_5->setEnabled(true);
        chbLandStr_6->setEnabled(true);
    } else {
        cbLandStrBelag_5->setEnabled(false);
        sbLandStrStart_5->setEnabled(false);
        sbLandStrEnd_5->setEnabled(false);
        chbLandStr_6->setEnabled(false);
        chbLandStr_6->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_6_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_6->setEnabled(true);
        sbLandStrStart_6->setEnabled(true);
        sbLandStrEnd_6->setEnabled(true);
        chbLandStr_7->setEnabled(true);
    } else {
        cbLandStrBelag_6->setEnabled(false);
        sbLandStrStart_6->setEnabled(false);
        sbLandStrEnd_6->setEnabled(false);
        chbLandStr_7->setEnabled(false);
        chbLandStr_7->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_7_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_7->setEnabled(true);
        sbLandStrStart_7->setEnabled(true);
        sbLandStrEnd_7->setEnabled(true);
        chbLandStr_8->setEnabled(true);
    } else {
        cbLandStrBelag_7->setEnabled(false);
        sbLandStrStart_7->setEnabled(false);
        sbLandStrEnd_7->setEnabled(false);
        chbLandStr_8->setEnabled(false);
        chbLandStr_8->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_8_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_8->setEnabled(true);
        sbLandStrStart_8->setEnabled(true);
        sbLandStrEnd_8->setEnabled(true);
        chbLandStr_9->setEnabled(true);
    } else {
        cbLandStrBelag_8->setEnabled(false);
        sbLandStrStart_8->setEnabled(false);
        sbLandStrEnd_8->setEnabled(false);
        chbLandStr_9->setEnabled(false);
        chbLandStr_9->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_9_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_9->setEnabled(true);
        sbLandStrStart_9->setEnabled(true);
        sbLandStrEnd_9->setEnabled(true);
        chbLandStr_10->setEnabled(true);
    } else {
        cbLandStrBelag_9->setEnabled(false);
        sbLandStrStart_9->setEnabled(false);
        sbLandStrEnd_9->setEnabled(false);
        chbLandStr_10->setEnabled(false);
        chbLandStr_10->setChecked(false);
    };
}

void ConfigureClass::on_chbLandStr_10_stateChanged(int arg1)
{
    if (arg1 == 2) {
        cbLandStrBelag_10->setEnabled(true);
        sbLandStrStart_10->setEnabled(true);
        sbLandStrEnd_10->setEnabled(true);
    } else {
        cbLandStrBelag_10->setEnabled(false);
        sbLandStrStart_10->setEnabled(false);
        sbLandStrEnd_10->setEnabled(false);
    };
}

void ConfigureClass::on_pbGenerateConfig_clicked()
{
    createSimuconf();
    writeSimutransSettings();
}

//QPixmap ConfigureClass::setThemeIcon(QString iconName)
//  {
//    KIconLoader *myIconLoader = new KIconLoader;

//    QPixmap setThemeIcon = myIconLoader->loadIcon(iconName,
//                                                  KIconLoader::Desktop,
//                                                  KIconLoader::SizeMedium,
//                                                  KIconLoader::DefaultState);
//    return setThemeIcon;

//  }

//void ConfigureClass::setIcons()
//  {
//    setWindowIcon(setThemeIcon("configure"));
//    tabMain->setTabIcon (0, setThemeIcon("simurun2"));
//    tabMain->setTabIcon (1, setThemeIcon(QString(KDE4_APPS) +
//                                         "/simurun2/images/pak-file"));
//    tabSettings->setTabIcon (0, setThemeIcon(QString(KDE4_APPS) +
//                                             "/simurun2/images/display"));
//    tabSettings->setTabIcon (1, setThemeIcon("network-connect"));
//    tabSettings->setTabIcon (2, setThemeIcon(QString(KDE4_APPS) +
//                                             "/simurun2/images/industrie"));
//    tabSettings->setTabIcon (3, setThemeIcon(QString(KDE4_APPS) +
//                                             "/simurun2/images/finanzen"));
//    tabSettings->setTabIcon (4, setThemeIcon(QString(KDE4_APPS) +
//                                             "/simurun2/images/strassen"));
//    pbDafault->setIcon (setThemeIcon("edit-undo"));
//    pbSpeichern->setIcon (setThemeIcon("dialog-ok-apply"));
//    pbGenerateConfig->setIcon (setThemeIcon("simutrans"));
//    pbSchliessen->setIcon (setThemeIcon("dialog-close"));
//    pbChDir->setIcon (setThemeIcon("folder"));
//    pbNetDelete->setIcon (setThemeIcon("trash-empty"));
//    pbNetAdd->setIcon (setThemeIcon("document-new"));
//    pbNetEdit->setIcon (setThemeIcon("document-edit"));
//    pbServerSave->setIcon (setThemeIcon("document-save"));
//    pbAendern->setIcon (setThemeIcon("dialog-ok-apply"));
//  }

void ConfigureClass::on_chb_Ziele_2_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_2->setEnabled(true);
        sb_Jahr_Ziel_2->setEnabled(true);
        chb_Ziele_3->setEnabled(true);
    } else {
        sb_Jahr_2->setEnabled(false);
        sb_Jahr_Ziel_2->setEnabled(false);
        chb_Ziele_3->setEnabled(false);
        chb_Ziele_3->setChecked(false);
    };
}
void ConfigureClass::on_chb_Ziele_3_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_3->setEnabled(true);
        sb_Jahr_Ziel_3->setEnabled(true);
        chb_Ziele_4->setEnabled(true);
    } else {
        sb_Jahr_3->setEnabled(false);
        sb_Jahr_Ziel_3->setEnabled(false);
        chb_Ziele_4->setEnabled(false);
        chb_Ziele_4->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_4_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_4->setEnabled(true);
        sb_Jahr_Ziel_4->setEnabled(true);
        chb_Ziele_5->setEnabled(true);
    } else {
        sb_Jahr_4->setEnabled(false);
        sb_Jahr_Ziel_4->setEnabled(false);
        chb_Ziele_5->setEnabled(false);
        chb_Ziele_5->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_5_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_5->setEnabled(true);
        sb_Jahr_Ziel_5->setEnabled(true);
        chb_Ziele_6->setEnabled(true);
    } else {
        sb_Jahr_5->setEnabled(false);
        sb_Jahr_Ziel_5->setEnabled(false);
        chb_Ziele_6->setEnabled(false);
        chb_Ziele_6->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_6_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_6->setEnabled(true);
        sb_Jahr_Ziel_6->setEnabled(true);
        chb_Ziele_7->setEnabled(true);
    } else {
        sb_Jahr_6->setEnabled(false);
        sb_Jahr_Ziel_6->setEnabled(false);
        chb_Ziele_7->setEnabled(false);
        chb_Ziele_7->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_7_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_7->setEnabled(true);
        sb_Jahr_Ziel_7->setEnabled(true);
        chb_Ziele_8->setEnabled(true);
    } else {
        sb_Jahr_7->setEnabled(false);
        sb_Jahr_Ziel_7->setEnabled(false);
        chb_Ziele_8->setEnabled(false);
        chb_Ziele_8->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_8_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_8->setEnabled(true);
        sb_Jahr_Ziel_8->setEnabled(true);
        chb_Ziele_9->setEnabled(true);
    } else {
        sb_Jahr_8->setEnabled(false);
        sb_Jahr_Ziel_8->setEnabled(false);
        chb_Ziele_9->setEnabled(false);
        chb_Ziele_9->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_9_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_9->setEnabled(true);
        sb_Jahr_Ziel_9->setEnabled(true);
        chb_Ziele_10->setEnabled(true);
    } else {
        sb_Jahr_9->setEnabled(false);
        sb_Jahr_Ziel_9->setEnabled(false);
        chb_Ziele_10->setEnabled(false);
        chb_Ziele_10->setChecked(false);
    };
}

void ConfigureClass::on_chb_Ziele_10_stateChanged(int arg1)
{
    if (arg1 == 2) {
        sb_Jahr_10->setEnabled(true);
        sb_Jahr_Ziel_10->setEnabled(true);
    } else {
        sb_Jahr_10->setEnabled(false);
        sb_Jahr_Ziel_10->setEnabled(false);
    };
}

void ConfigureClass::on_cbServerAnnounce_currentIndexChanged(int index)
{
    if(index == 1)
    {
        sbAboIntervall->setEnabled(true);
    }
    else
    {
        sbAboIntervall->setEnabled(false);
    }
}

void ConfigureClass::readServerXml()
{
    readNetGamesXml.clear();

    QFile readXML(APP_CONFIG_PATH + "/gameserver.xml");
    readXML.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray line = readXML.readAll();
    readNetGamesXml.addData(line);

    while (!readNetGamesXml.atEnd())
    {
        readNetGamesXml.readNext();
        if (readNetGamesXml.isStartElement())
        {
            if (readNetGamesXml.name() == "server")
                serverStr = readNetGamesXml.attributes().value("href").toString();
            currentTag = readNetGamesXml.name().toString();
        }
        else if (readNetGamesXml.isEndElement())
        {
            if (readNetGamesXml.name() == "server")
            {
                QTableWidgetItem *itemReadXml1 = new QTableWidgetItem;
                itemReadXml1->setData(0, serverStr);

                QTableWidgetItem *itemReadXml2 = new QTableWidgetItem;
                itemReadXml2->setData(0, paksetStr);

                QTableWidgetItem *itemReadXml3 = new QTableWidgetItem;
                itemReadXml3->setData(0, descStr);

                int xmlRows = serverTableWidget->rowCount();
                serverTableWidget->setWordWrap(true);
                serverTableWidget->insertRow(xmlRows);

                serverTableWidget->setItem(xmlRows, 0, itemReadXml1);
                serverTableWidget->setItem(xmlRows, 1, itemReadXml2);
                serverTableWidget->setItem(xmlRows, 2, itemReadXml3);
                serverTableWidget->resizeRowsToContents();
                serverTableWidget->resizeColumnsToContents();

                serverStr.clear();
                paksetStr.clear();
                descStr.clear();
            }

        }
        else if (readNetGamesXml.isCharacters() && !readNetGamesXml.isWhitespace())
        {
            if (currentTag == "href")
                serverStr += readNetGamesXml.text().toString();
            else if (currentTag == "pakset")
                paksetStr += readNetGamesXml.text().toString();
            else if (currentTag == "desc")
                descStr += readNetGamesXml.text().toString();
        }
    }
    readXML.close();
}

void ConfigureClass::addXmlItems(QString server, QString pakset, QString desc)
{
    QTableWidgetItem *itemWriteXml1 = new QTableWidgetItem;
    itemWriteXml1->setData(0, server);

    QTableWidgetItem *itemWriteXml2 = new QTableWidgetItem;
    itemWriteXml2->setData(0, pakset);

    QTableWidgetItem *itemWriteXml3 = new QTableWidgetItem;
    itemWriteXml3->setData(0, desc);

    int xmlRows = serverTableWidget->rowCount();
    serverTableWidget->setWordWrap(true);
    serverTableWidget->insertRow(xmlRows);

    serverTableWidget->setItem(xmlRows, 0, itemWriteXml1);
    serverTableWidget->setItem(xmlRows, 1, itemWriteXml2);
    serverTableWidget->setItem(xmlRows, 2, itemWriteXml3);

    serverTableWidget->resizeRowsToContents();
    //    serverTableWidget->resizeColumnsToContents();
}

void ConfigureClass::writeServerXml()
{

    int mRows = serverTableWidget->rowCount();

    QFile saveXML(APP_CONFIG_PATH + "/gameserver.xml");
    saveXML.open(QIODevice::WriteOnly | QIODevice::Text);

    QXmlStreamWriter writeNetGamesXml(&saveXML );
    writeNetGamesXml.setAutoFormatting(true);
    writeNetGamesXml.writeStartDocument();
    writeNetGamesXml.writeStartElement("gameserver");
    writeNetGamesXml.writeComment(tr("Diese Datei nicht bearbeiten oder löschen!"));

    for (int i = 0; i < mRows ; ++i)
    {
        writeNetGamesXml.writeStartElement("server");
        writeNetGamesXml.writeAttribute("href", serverTableWidget->item(i, 0)->text());
        writeNetGamesXml.writeTextElement("pakset", serverTableWidget->item(i, 1)->text());
        writeNetGamesXml.writeTextElement("desc", serverTableWidget->item(i, 2)->text());
        writeNetGamesXml.writeEndElement();
    }
    writeNetGamesXml.writeEndElement();
    saveXML.close();
}

void ConfigureClass::on_pbNetAdd_clicked()
{
    editServerFrame->setVisible(true);
    pbAendern ->setVisible(false);
    txt_Server->clear();
    txt_Pakset->clear();
    txt_Desc->clear();
    pbNetAdd->setEnabled(false);
}

void ConfigureClass::on_pbNetEdit_clicked()
{
    editServerFrame->setVisible(true);
    pbAendern ->setVisible(true);
    txt_Server->setText(serverTableWidget->item(serverTableWidget->currentRow(), 0)->text());
    txt_Pakset->setText(serverTableWidget->item(serverTableWidget->currentRow(), 1)->text());
    txt_Desc->setText(serverTableWidget->item(serverTableWidget->currentRow(), 2)->text());
    pbNetEdit->setEnabled(false);
}

void ConfigureClass::on_pbNetDelete_clicked()
{
    serverTableWidget->removeRow(serverTableWidget->currentRow());
    pbAendern ->setVisible(false);
    editServerFrame->setVisible(false);
    writeServerXml();
}

void ConfigureClass::on_txt_Server_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() == false) {
        txt_Pakset->setEnabled(true);
        txt_Desc->setEnabled(true);
    } else if(arg1.isEmpty() == true) {
        txt_Pakset->setEnabled(false);
        txt_Desc->setEnabled(false);
    }
}

void ConfigureClass::on_serverTableWidget_cellClicked()
{
    pbNetEdit->setEnabled(true);
    pbNetDelete->setEnabled(true);
}

void ConfigureClass::on_pbAendern_clicked()
{
    pbAendern ->setVisible(false);
    editServerFrame->setVisible(false);
    serverTableWidget->item(serverTableWidget->currentRow(), 0)->setText(txt_Server->text());
    serverTableWidget->item(serverTableWidget->currentRow(), 1)->setText(txt_Pakset->text());
    serverTableWidget->item(serverTableWidget->currentRow(), 2)->setText(txt_Desc->text());
    txt_Server->clear();
    txt_Pakset->clear();
    txt_Desc->clear();
    writeServerXml();
}
