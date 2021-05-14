#lessThan(QT_VERSION, 5.7) {
#    error("Program requires at least Qt 5.7!")
#}

QT += core
QT += widgets
QT += xml

INCLUDEPATH += /usr/lib64

TEMPLATE = app
VERSION = -3.0-alpha
LANGUAGE += C++
CODECFORTR = UTF-8
#DEFINES += QT
CONFIG += qt release
MOC_DIR += moc
UI_DIR += ui
OBJECTS_DIR += obj
TARGET = simurun
target.path += /usr/bin

# Diese Verzeichnisse beim generieren von *.ts Übersetzungsdateien nicht berücksichtigen.
TR_EXCLUDE += \
    qtsingleapplication/*

include(qtsingleapplication/src/qtsingleapplication.pri)

DEPENDPATH += . \
    doc \
    ui \
    src \
    translations

HEADERS += \
    src/definitionen.h \
    src/globfunction.h \
    src/setupgame.h \
    src/imageview.h \
    src/languagetools.h \
    src/configureclass.h \
    src/updates.h \
    src/helpbrowser.h \
    src/simurun.h \
    src/about.h \
    src/parse_xml_local.h \
    src/parse_xml_net.h

FORMS += \
    ui/imageview.ui \
    ui/configureclass.ui \
    ui/updates.ui \
    ui/helpbrowser.ui \
    ui/about.ui \
    ui/simurun.ui

SOURCES += src/main.cpp \
    src/globfunction.cpp \
    src/setupgame.cpp \
    src/imageview.cpp \
    src/languagetools.cpp \
    src/configureclass.cpp \
    src/updates.cpp \
    src/helpbrowser.cpp \
    src/simurun.cpp \
    src/about.cpp \
    src/parse_xml_local.cpp \
    src/parse_xml_net.cpp

RESOURCES += simurun2.qrc

OTHER_FILES += ChangeLog \
    INSTALL \
    NEWS \
    TODO \
    my_rpm \
    AUTHORS \
    translations/README \
    tpl/complette_simuconf.txt \
    simurun.desktop \
    simurun.spec \
    translations/translation_info.txt

TRANSLATIONS += \
    translations/simurun_de.ts \
    translations/simurun_en.ts \
    translations/simurun_nl.ts \
    translations/simurun_es.ts \
    translations/simurun_it.ts \
    translations/simurun_pt.ts

DISTFILES += \
    translations/simurun_de.qm \
    translations/simurun_en.qm \
    translations/simurun_nl.qm \
    translations/simurun_it.qm \
    translations/simurun_es.qm \
    translations/simurun_pt.qm \
    AUTHORS \
    ChangeLog \
    COPYING \
    INSTALL \
    NEWS \
    TODO \
    images/flags/* \
#    images/icons/* \
    tpl/* \
    doc/* \
#    doc/en/* \
#    doc/nl/* \
#    doc/pt/* \
#    doc/es/* \
#    doc/it/* \
    simurun2.spec \
    translations/translation_info_en.txt \
    translations/translation_info_de.txt \
    translations/README \
    README.md

##### Icons
pixmapSIMU.path =  /usr/share/pixmaps
pixmapSIMU.files = images/simurun2.png \
                    images/simutrans.png

##### Desktop file
desktopfile.path = /usr/share/applications/
desktopfile.files = $(TARGET).desktop

##### Flags
flags.path += /usr/share/doc/packages/$(TARGET)/flags/
flags.files += images/flags/*

##### Translations
translation.path += /usr/share/doc/packages/$(TARGET)/locale/
translation.files += translations/*
#translationEN.path += /usr/share/locale/en/LC_MESSAGES
#translationEN.files += 	translations/$(TARGET)_en.qm
# translationNL.path += /usr/share/locale/nl/LC_MESSAGES
# translationNL.files += 	translations/$(TARGET)_nl.qm
# translationPT.path += /usr/share/locale/pt/LC_MESSAGES
# translationPT.files += 	translations/$(TARGET)_pt.qm
# translationES.path += /usr/share/locale/es/LC_MESSAGES
# translationES.files += 	translations/$(TARGET)_es.qm
# translationIT.path += /usr/share/locale/it/LC_MESSAGES
# translationIT.files += 	translations/$(TARGET)_it.qm

##### Documentation
help.path += /usr/share/doc/packages/$(TARGET)/doc/
help.files += doc/*
#helpEN.path += /usr/share/doc/packages/$(TARGET)
#helpEN.files += doc/en/index.cache.bz2 \
#                doc/en/index.docbook
# helpNL.path += /usr/share/doc/packages/$(TARGET)
# helpNL.files += doc/nl/index.cache.bz2 \
#                 doc/nl/index.docbook
# helpPT.path += /usr/share/doc/packages/$(TARGET)
# helpPT.files += doc/pt/index.cache.bz2 \
#                 doc/pt/index.docbook
# helpIT.path += /usr/share/doc/packages/$(TARGET)
# helpIT.files += doc/it/index.cache.bz2 \
#                 doc/it/index.docbook
# helpES.path += /usr/share/doc/packages/$(TARGET)
# helpES.files += doc/es/index.cache.bz2 \
#                 doc/es/index.docbook

##### Templates
templates.path = /usr/share/doc/packages/$(TARGET)/tpl/
templates.files += tpl/*

##### Other files
documentation.path = /usr/share/doc/packages/$(TARGET)
documentation.files += AUTHORS \
    ChangeLog \
    COPYING \
    INSTALL \
    NEWS \
    README \
    TODO

INSTALLS += target \
    documentation \
    templates \
    pixmapSIMU \
    flags \
    help \
#    helpEN \
#    helpNL \
#    helpPT \
#    helpIT \
#    helpES \
    translation \
#    translationEN \
#    translationNL \
#    translationPT \
#    translationIT \
#    translationES \
    desktopfile


CONFIG(debug, debug|release) {
#    QMAKE_CXXFLAGS_DEBUG += -g3 -O0
    message("DEBUG!")
} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    message("RELEASE!")
}
