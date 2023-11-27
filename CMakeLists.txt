# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(aphototoollibre)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    src
    src/fileio
    src/filters
    src/utils
    src/validators
    src/values
)

qt_wrap_ui(SOURCES aphototoollibre.ui optionsdialog.ui)

set (SRCS
    src/fileio/basefile.cpp
    src/fileio/basefile.h
    src/fileio/openfile.cpp
    src/fileio/openfile.h
    src/fileio/savefile.cpp
    src/fileio/savefile.h
    src/filters/brightnessfilter.cpp
    src/filters/brightnessfilter.h
    src/filters/bwfilter.cpp
    src/filters/bwfilter.h
    src/filters/colortemperaturefilter.cpp
    src/filters/colortemperaturefilter.h
    src/filters/contrastfilter.cpp
    src/filters/contrastfilter.h
    src/filters/cropfilter.cpp
    src/filters/cropfilter.h
    src/filters/cropvalues.cpp
    src/filters/cropvalues.h
    src/filters/filters.cpp
    src/filters/filters.h
    src/filters/filtervalues.cpp
    src/filters/filtervalues.h
    src/filters/huefilter.cpp
    src/filters/huefilter.h
    src/filters/lightdarkfilter.cpp
    src/filters/lightdarkfilter.h
    src/filters/resizefilter.cpp
    src/filters/resizefilter.h
    src/filters/rotateanyanglefilter.cpp
    src/filters/rotateanyanglefilter.h
    src/filters/saturationfilter.cpp
    src/filters/saturationfilter.h
    src/filters/tintfilter.cpp
    src/filters/tintfilter.h
    src/resources/Welcome.png
    src/utils/backgroundcontrol.cpp
    src/utils/backgroundcontrol.h
    src/utils/graphics.cpp
    src/utils/graphics.h
    src/utils/histogram.cpp
    src/utils/histogram.h
    src/utils/imagecreator.cpp
    src/utils/imagecreator.h
    src/utils/print.cpp
    src/utils/print.h
    src/utils/qpixmapversionedutils.cpp
    src/utils/qpixmapversionedutils.h
    src/utils/settingsutils.cpp
    src/utils/settingsutils.h
    src/utils/stylemode.cpp
    src/utils/stylemode.h
    src/utils/timeutil.cpp
    src/utils/timeutil.h
    src/validators/cropareavalidator.cpp
    src/validators/cropareavalidator.h
    src/values/imagevalues.cpp
    src/values/imagevalues.h
    src/values/usersettingvalues.cpp
    src/values/usersettingvalues.h
    src/values/workvalues.cpp
    src/values/workvalues.h
    src/aphototoollibre.cpp
    src/aphototoollibre.h
    src/aphototoollibre.ui
    src/constants.h
    src/croptoolui.cpp
    src/croptoolui.h
    src/imagelabel.cpp
    src/imagelabel.h
    src/imagelabelpainter.cpp
    src/imagelabelpainter.h
    src/main.cpp
    src/optionsdialog.cpp
    src/optionsdialog.h
    src/optionsdialog.ui
    src/resizetoolui.cpp
    src/resizetoolui.h
    src/rotatetoolui.cpp
    src/rotatetoolui.h
    src/showimageevent.cpp
    src/showimageevent.h
)



add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)


install(TARGETS aphototoollibre
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES debian/aphototoollibre.1.gz DESTINATION share/man/man1
)
install(
    FILES desktop-std/me.ahola.aphototoollibre.desktop DESTINATION share/applications
)
install(
    FILES desktop-std/me.ahola.aphototoollibre.svg DESTINATION share/icons/hicolor/scalable/apps
)
install(
    FILES desktop-std/me.ahola.aphototoollibre.metainfo.xml DESTINATION share/metainfo
)

