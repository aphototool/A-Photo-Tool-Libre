#ifndef SETTINGSUTILS_H
#define SETTINGSUTILS_H

#include <QMainWindow>
#include <QSettings>
#include "../values/usersettingvalues.h"

class SettingsUtils
{
public:
    SettingsUtils();
    static void writeSettings(UserSettingValues *appSettings, QMainWindow *window);
    static void readSettings(UserSettingValues *appSettings, QMainWindow *window);
};

#endif // SETTINGSUTILS_H
