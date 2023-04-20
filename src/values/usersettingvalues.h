/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2022 Jari Ahola
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

#ifndef USERSETTINGVALUES_H
#define USERSETTINGVALUES_H


class UserSettingValues
{
public:
    UserSettingValues();

    bool getDarkMode() const;
    void setDarkMode(bool newDarkMode);

    bool getShowHistogram() const;
    void setShowHistogram(bool newShowHistogram);

    bool getFirstUse() const;
    void setFirstUse(bool newFirstUse);

private:
    bool darkMode = false;
    bool showHistogram = false;
    bool firstUse = false;
};

#endif // USERSETTINGVALUES_H
