#include "settingsutils.h"

SettingsUtils::SettingsUtils()
{

}

void SettingsUtils::writeSettings(UserSettingValues *appSettings, QMainWindow *window)
{
    QSettings settings("aphototoollibre", "config");

    settings.beginGroup("Window");
    settings.setValue("darkMode", appSettings->getDarkMode());
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
    appSettings->setFirstUse(settings.value("firstUse", true).toBool());
    settings.endGroup();

    settings.beginGroup("MainWindow");
    window->resize(settings.value("size", QSize(1200, 600)).toSize());
    window->move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}
