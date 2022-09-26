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

#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "aphototoollibre.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->lightModeRadioButton, &QRadioButton::clicked, this, &OptionsDialog::lightMode);
    QObject::connect(ui->darkModeRadioButton, &QRadioButton::clicked, this, &OptionsDialog::darkMode);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::lightMode() {
    useDarkMode(false);
}

void OptionsDialog::darkMode() {
    useDarkMode(true);
}

void OptionsDialog::useDarkMode(bool darkMode) {
    APhotoToolLibre *parent = (APhotoToolLibre*)parentClass;
    parent->getAppSettings()->setDarkMode(darkMode);
    if (darkMode) {
        StyleMode::darkMode(qApp, parent->getUi());
    } else {
        StyleMode::lightMode(qApp, parent->getUi());
    }
    SettingsUtils::writeSettings(parent->getAppSettings(), parent);
}

void OptionsDialog::prepareSettingsDialog(QMainWindow *newParentClass)
{
    setWindowTitle(tr("Options"));
    parentClass = newParentClass;
    bool darkMode = ((APhotoToolLibre*)parentClass)->getAppSettings()->getDarkMode();
    ui->lightModeRadioButton->setChecked(!darkMode);
    ui->darkModeRadioButton->setChecked(darkMode);
}

