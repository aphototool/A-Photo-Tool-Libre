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
}

void OptionsDialog::prepareSettingsDialog(QMainWindow *newParentClass)
{
    setWindowTitle(tr("Options"));
    parentClass = newParentClass;
    bool darkMode = ((APhotoToolLibre*)parentClass)->getAppSettings()->getDarkMode();
    ui->lightModeRadioButton->setChecked(!darkMode);
    ui->darkModeRadioButton->setChecked(darkMode);
}

