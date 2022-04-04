#ifndef USERSETTINGS_H
#define USERSETTINGS_H


class UserSettings
{
public:
    UserSettings();

    bool getDarkMode() const;
    void setDarkMode(bool newDarkMode);

private:
    bool darkMode = false;
};

#endif // USERSETTINGS_H
