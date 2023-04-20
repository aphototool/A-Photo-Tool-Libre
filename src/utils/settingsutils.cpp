/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2023 Jari Ahola
 * GNU General Public License (GPLv3)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "settingsutils.h"

SettingsUtils::SettingsUtils()
{

}

void SettingsUtils::writeSettings(UserSettingValues *appSettings, QMainWindow *window)
{
    QSettings settings("aphototoollibre", "config");

    settings.beginGroup("Window");
    settings.setValue("darkMode", appSettings->getDarkMode());
    settings.setValue("showHistogram", appSettings->getShowHistogram());
    settings.setValue("firstUse", false);
    settings.endGroup();

    settings.beginGroup("MainWindow");
    settings.setValue("size", window->size());
    settings.setValue("pos", window->pos());
    settings.endGroup();
}

void SettingsUtils::readSettings(UserSettingValues *appSettings, QMainWindow *window)
{
    QSettings settings("aphototoollibre", "config");

    settings.beginGroup("Window");
    appSettings->setDarkMode(settings.value("darkMode").toBool());
    appSettings->setShowHistogram(settings.value("showHistogram").toBool());
    appSettings->setFirstUse(settings.value("firstUse", true).toBool());
    settings.endGroup();

    settings.beginGroup("MainWindow");
    window->resize(settings.value("size", QSize(1200, 600)).toSize());
    window->move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}
