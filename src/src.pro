QT       += core gui

qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# For use with Qt 6
#CONFIG += c++11

TARGET = aphototoollibre
TEMPLATE = app
CONFIG += warn_on

# TRANSLATIONS = aphototoollibre_en_US.ts

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x051500

SOURCES += \
    aphototoollibre.cpp \
    croptoolui.cpp \
    fileio/basefile.cpp \
    fileio/openfile.cpp \
    fileio/savefile.cpp \
    filters/brightnessfilter.cpp \
    filters/bwfilter.cpp \
    filters/colortemperaturefilter.cpp \
    filters/contrastfilter.cpp \
    filters/cropfilter.cpp \
    filters/cropvalues.cpp \
    filters/filters.cpp \
    filters/filtervalues.cpp \
    filters/huefilter.cpp \
    filters/lightdarkfilter.cpp \
    filters/resizefilter.cpp \
    filters/rotateanyanglefilter.cpp \
    filters/saturationfilter.cpp \
    imagelabel.cpp \
    imagelabelpainter.cpp \
    utils/backgroundcontrol.cpp \
    utils/imagecreator.cpp \
    utils/settingsutils.cpp \
    utils/timeutil.cpp \
    values/imagevalues.cpp \
    main.cpp \
    optionsdialog.cpp \
    resizetoolui.cpp \
    rotatetoolui.cpp \
    showimageevent.cpp \
    utils/graphics.cpp \
    utils/print.cpp \
    utils/stylemode.cpp \
    validators/cropareavalidator.cpp \
    values/usersettingvalues.cpp \
    values/workvalues.cpp

HEADERS += \
    aphototoollibre.h \
    constants.h \
    croptoolui.h \
    fileio/basefile.h \
    fileio/openfile.h \
    fileio/savefile.h \
    filters/brightnessfilter.h \
    filters/bwfilter.h \
    filters/colortemperaturefilter.h \
    filters/contrastfilter.h \
    filters/cropfilter.h \
    filters/cropvalues.h \
    filters/filters.h \
    filters/filtervalues.h \
    filters/huefilter.h \
    filters/lightdarkfilter.h \
    filters/resizefilter.h \
    filters/rotateanyanglefilter.h \
    filters/saturationfilter.h \
    imagelabel.h \
    imagelabelpainter.h \
    utils/backgroundcontrol.h \
    utils/imagecreator.h \
    utils/settingsutils.h \
    utils/timeutil.h \
    values/imagevalues.h \
    optionsdialog.h \
    resizetoolui.h \
    rotatetoolui.h \
    showimageevent.h \
    utils/graphics.h \
    utils/print.h \
    utils/stylemode.h \
    validators/cropareavalidator.h \
    values/usersettingvalues.h \
    values/workvalues.h

FORMS += \
    aphototoollibre.ui \
    optionsdialog.ui

unix: {

target.path = /usr/bin/
INSTALLS += target

unix_man.path = /usr/share/man/man1
unix_man.files = ../debian/aphototoollibre.1.gz
INSTALLS += unix_man

unix_desktop.path = /usr/share/applications
unix_desktop.files = ../desktop/aphototoollibre.desktop
INSTALLS += unix_desktop

unix_icons.path = /usr/share/icons/hicolor/scalable/apps
unix_icons.files = ../desktop/aphototoollibre.svg
INSTALLS += unix_icons

unix_metainfo.path = /usr/share/metainfo
unix_metainfo.files = ../desktop/aphototoollibre.metainfo.xml
INSTALLS += unix_metainfo

}

# END
