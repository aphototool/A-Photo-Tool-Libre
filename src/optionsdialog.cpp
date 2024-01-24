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
    QObject::connect(ui->histogramCheckBox, &QCheckBox::clicked, this, &OptionsDialog::showHistogram);

    ui->tabWidget->setCurrentIndex(0);
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

void OptionsDialog::showHistogram() {
    APhotoToolLibre *parent = (APhotoToolLibre*)parentClass;
    bool show = ui->histogramCheckBox->isChecked();
    parent->getAppSettings()->setShowHistogram(show);
    parent->showHistogram(show);
    SettingsUtils::writeSettings(parent->getAppSettings(), parent);
}


void OptionsDialog::useDarkMode(bool darkMode) {
    APhotoToolLibre *parent = (APhotoToolLibre*)parentClass;
    parent->getAppSettings()->setDarkMode(darkMode);
    parent->useDarkMode(darkMode);
    SettingsUtils::writeSettings(parent->getAppSettings(), parent);
}

void OptionsDialog::prepareSettingsDialog(QMainWindow *newParentClass)
{
    setWindowTitle(tr("Options"));
    parentClass = newParentClass;
    bool darkMode = ((APhotoToolLibre*)parentClass)->getAppSettings()->getDarkMode();
    ui->lightModeRadioButton->setChecked(!darkMode);
    ui->darkModeRadioButton->setChecked(darkMode);
    ui->histogramCheckBox->setChecked(((APhotoToolLibre*)parentClass)->getAppSettings()->getShowHistogram());
}

