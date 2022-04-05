#include "usersettingvalues.h"

UserSettingValues::UserSettingValues()
{

}

bool UserSettingValues::getDarkMode() const
{
    return darkMode;
}

void UserSettingValues::setDarkMode(bool newDarkMode)
{
    darkMode = newDarkMode;
}
