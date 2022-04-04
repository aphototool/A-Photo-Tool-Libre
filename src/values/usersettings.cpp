#include "usersettings.h"

UserSettings::UserSettings()
{

}

bool UserSettings::getDarkMode() const
{
    return darkMode;
}

void UserSettings::setDarkMode(bool newDarkMode)
{
    darkMode = newDarkMode;
}
