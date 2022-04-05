#ifndef USERSETTINGVALUES_H
#define USERSETTINGVALUES_H


class UserSettingValues
{
public:
    UserSettingValues();

    bool getDarkMode() const;
    void setDarkMode(bool newDarkMode);

private:
    bool darkMode = false;
};

#endif // USERSETTINGVALUES_H
