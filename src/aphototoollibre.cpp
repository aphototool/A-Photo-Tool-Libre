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

#include "aphototoollibre.h"
#include "./ui_aphototoollibre.h"

APhotoToolLibre::APhotoToolLibre(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    values = Values();

    QObject::connect(ui->loadImageButton, &QPushButton::clicked, this, &APhotoToolLibre::onLoadImageButtonClicked);
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &APhotoToolLibre::onLoadImageButtonClicked);
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &APhotoToolLibre::onSaveButtonClicked);
    QObject::connect(ui->actionSave, &QAction::triggered, this, &APhotoToolLibre::onSaveButtonClicked);
    QObject::connect(ui->actionPrint, &QAction::triggered, this, &APhotoToolLibre::onPrintClicked);
    QObject::connect(ui->actionCloseWindow, &QAction::triggered, this, &APhotoToolLibre::onCloseWindowClicked);
    QObject::connect(ui->resetExposureButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetExposureClicked);
    QObject::connect(ui->resetColorsButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetColorsClicked);
    QObject::connect(ui->brightnessSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onBrightnessSliderValueChanged);
    QObject::connect(ui->contrastSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onContrastSliderValueChanged);
    QObject::connect(ui->lightsSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onLightsSliderValueChanged);
    QObject::connect(ui->darksSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onDarksSliderValueChanged);
    QObject::connect(ui->saturationSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onSaturationSliderValueChanged);
    QObject::connect(ui->hueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onHueSliderValueChanged);
    QObject::connect(ui->colorTemperatureSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onColorTemperatureSliderValueChanged);
    QObject::connect(ui->naturalButton, &QPushButton::clicked, this, &APhotoToolLibre::onNaturalButtonClicked);
    QObject::connect(ui->panchromaticButton, &QPushButton::clicked, this, &APhotoToolLibre::onPanchromaticButtonClicked);
    QObject::connect(ui->enableBWCheckBox, &QCheckBox::stateChanged, this, &APhotoToolLibre::onEnableBWCheckBoxStateChanged);
    QObject::connect(ui->redSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onRedSliderValueChanged);
    QObject::connect(ui->greenSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onGreenSliderValueChanged);
    QObject::connect(ui->blueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onBlueSliderValueChanged);
    QObject::connect(ui->actionOptions, &QAction::triggered, this, &APhotoToolLibre::showSettings);
    QObject::connect(&workValues.filterWatcher, &QFutureWatcherBase::finished, this, &APhotoToolLibre::backgroundFilterReady);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &APhotoToolLibre::about);
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    ui->actionPrint->setEnabled(true);
#else
    ui->actionPrint->setEnabled(false);
#endif

    rotateToolUi = new RotateToolUi(this, ui, &values, &workValues);
    cropToolUi = new CropToolUi(this, ui, &values, &workValues);
    resizeToolUi = new ResizeToolUi(this, ui, &values);

    ui->mainToolsWidget->setCurrentIndex(0);

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(false);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    resetValues();

    readSettings();
    if (appSettings.getDarkMode()) {
        StyleMode::darkMode(qApp, ui);
    } else {
        StyleMode::lightMode(qApp, ui);
    }

    QStringList args = qApp->arguments();
    for (int i = 0; i < args.size(); i++) {
        QString arg = args.at(i);
        if (arg.endsWith(".jpg", Qt::CaseInsensitive) ||
                arg.endsWith(".jpeg", Qt::CaseInsensitive) ||
                arg.endsWith(".png", Qt::CaseInsensitive))  {
            argFileName = arg;
            QTimer::singleShot(250, this, [this]() { loadImage(this->argFileName); } );
            break;
        }
    }

    QTimer *backgroundFilterTimer = new QTimer(this);
    connect(backgroundFilterTimer, &QTimer::timeout, this, [this]() { createFullResolutionInBackground(); } );
    backgroundFilterTimer->start(2000);
}

void APhotoToolLibre::closeEvent(QCloseEvent *event)
{
    if (values.image.height() > 0 && values.imageModified) {
        if (QMessageBox::No == QMessageBox(QMessageBox::Information, tr("Close Window?"), tr("Close window and discard unsaved edits."), QMessageBox::Yes|QMessageBox::No, this).exec())
        {
            event->ignore();
            return;
        }
    }
    writeSettings();
    event->accept();
}

APhotoToolLibre::~APhotoToolLibre()
{
    delete ui;
}

void APhotoToolLibre::onCloseWindowClicked() {
    close();
}

void APhotoToolLibre::resetValues() {
    values.filterValues = FilterValues();
    setBWSliders();
    ui->enableBWCheckBox->setChecked(values.filterValues.enableBW);
    setExposureSliders();
    setColorSliders();
    ui->rotateFrame->setVisible(false);
    ui->cropFrame->setVisible(false);
    ui->resizeFrame->setVisible(false);
    // Set imageModified to false after other settings
    // to avoid unwanted state changes by slider resets.
    values.imageModified = false;
}

void APhotoToolLibre::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (ui->rotateFrame->isVisible()) {
        rotateToolUi->resizeEvent(event);
    } else if (ui->cropFrame->isVisible()) {
        cropToolUi->resizeEvent(event);
    } else {
        showPreviewImage();
    }
}

void APhotoToolLibre::mousePressEvent(QMouseEvent *event)
{
    QMainWindow::mousePressEvent(event);
    cropToolUi->mousePressEvent(event);
}

void APhotoToolLibre::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
    cropToolUi->mouseMoveEvent(event);
}

void APhotoToolLibre::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);
    cropToolUi->mouseReleaseEvent(event);
}

void APhotoToolLibre::loadImage(const QString fileName)
{
    if (isLoadNewPhtoOk()) {
        OpenFile file;
        QImage tempImage = file.open(fileName);
        if (tempImage.format() == QImage::Format_Invalid) return;
        loadImage(fileName, tempImage);
    }
}

void APhotoToolLibre::onLoadImageButtonClicked()
{
    if (isLoadNewPhtoOk()) {
        OpenFile file;
        QImage tempImage = file.open();
        if (tempImage.format() == QImage::Format_Invalid) return;
        loadImage(file.getFileName(), tempImage);
    }
}

bool APhotoToolLibre::isLoadNewPhtoOk() {
    if (values.image.height() > 0 && values.imageModified) {
        if (QMessageBox::No == QMessageBox(QMessageBox::Information, tr("Replace Image?"), tr("Load new photo and discard unsaved edits."), QMessageBox::Yes|QMessageBox::No, this).exec())
        {
            return false;
        }
    }
    return true;
}

void APhotoToolLibre::loadImage(QString fileName, QImage tempImage) {
    resetValues();
    ImageCreator::createPreviewImage(tempImage, values);
    showFullResolutionImage();
    values.originaFileName = fileName;
    showFileInfo();
}

void APhotoToolLibre::onSaveButtonClicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->previewLabel->setText("Working...");
    QTimer::singleShot(100, this, [this]() { saveFullResImage(); } );
}

void APhotoToolLibre::saveFullResImage()
{
    showFullResolutionImage();
    QApplication::restoreOverrideCursor();
    SaveFile file;
    file.saveAs(&values);
}

void APhotoToolLibre::onResetExposureClicked()
{
    values.filterValues.brightness = 0.0;
    values.filterValues.contrast = 0.0;
    values.filterValues.lights = 0.0;
    values.filterValues.darks = 0.0;
    setExposureSliders();
    showPreviewImage();
}

void APhotoToolLibre::onResetColorsClicked()
{
    values.filterValues.saturation = 0.0;
    values.filterValues.hue = 0.0;
    values.filterValues.colorTemperature = 0.0;
    setColorSliders();
    showPreviewImage();
}

void APhotoToolLibre::onEnableBWCheckBoxStateChanged(int arg1)
{
    values.filterValues.enableBW = bool(arg1);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onRedSliderValueChanged(int value)
{
    values.filterValues.redDelta = float(value);
    values.imageModified = true;
    setBWValueLabels();
    showPreviewImage();
}

void APhotoToolLibre::onGreenSliderValueChanged(int value)
{
    values.filterValues.greenDelta = float(value);
    values.imageModified = true;
    setBWValueLabels();
    showPreviewImage();
}

void APhotoToolLibre::onBlueSliderValueChanged(int value)
{
    values.filterValues.blueDelta = float(value);
    values.imageModified = true;
    setBWValueLabels();
    showPreviewImage();
}

void APhotoToolLibre::onNaturalButtonClicked() {
    values.filterValues.redDelta = 30.0;
    values.filterValues.greenDelta = 60.0;
    values.filterValues.blueDelta = 10.0;
    values.imageModified = true;
    setBWSliders();
    showPreviewImage();
}

void APhotoToolLibre::onPanchromaticButtonClicked() {
    values.filterValues.redDelta = 50.0;
    values.filterValues.greenDelta = 50.0;
    values.filterValues.blueDelta = 50.0;
    values.imageModified = true;
    setBWSliders();
    showPreviewImage();
}

void APhotoToolLibre::onBrightnessSliderValueChanged(int value)
{
    values.filterValues.brightness = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onContrastSliderValueChanged(int value)
{
    values.filterValues.contrast = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onLightsSliderValueChanged(int value)
{
    values.filterValues.lights = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onDarksSliderValueChanged(int value)
{
    values.filterValues.darks = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onSaturationSliderValueChanged(int value)
{
    values.filterValues.saturation = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onHueSliderValueChanged(int value)
{
    values.filterValues.hue = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onColorTemperatureSliderValueChanged(int value)
{
    values.filterValues.colorTemperature = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::setExposureSliders()
{
    ui->brightnessSlider->setValue(values.filterValues.brightness);
    ui->contrastSlider->setValue(values.filterValues.contrast);
    ui->lightsSlider->setValue(values.filterValues.lights);
    ui->darksSlider->setValue(values.filterValues.darks);
}

void APhotoToolLibre::setColorSliders()
{
    ui->saturationSlider->setValue(values.filterValues.saturation);
    ui->hueSlider->setValue(values.filterValues.hue);
    ui->colorTemperatureSlider->setValue(values.filterValues.colorTemperature);
}

void APhotoToolLibre::setBWSliders() {
    ui->redSlider->setValue(values.filterValues.redDelta);
    ui->greenSlider->setValue(values.filterValues.greenDelta);
    ui->blueSlider->setValue(values.filterValues.blueDelta);
    setBWValueLabels();
}

void APhotoToolLibre::setBWValueLabels()
{
    ui->redLabel->setText(QString("%1").arg(int(values.filterValues.redDelta)));
    ui->greenLabel->setText(QString("%1").arg(int(values.filterValues.greenDelta)));
    ui->blueLabel->setText(QString("%1").arg(int(values.filterValues.blueDelta)));
}

void APhotoToolLibre::showFileInfo()
{
    Graphics::showFileInfo(values, ui->infoLabel, this);
}

void APhotoToolLibre::showPreviewImage()
{
    QImage tempImage = values.imageOriginalScaled;
    ResizeCalcType resizeCalcTypeSaved = values.filterValues.resizeType;
    // No resizing for preview image
    values.filterValues.resizeType = ResizeCalcType::None;
    showImage(tempImage);
    values.filterValues.resizeType = resizeCalcTypeSaved;
    workValues.filterMutex.lock();
    values.filteredTime = TimeUtil::getTimestamp();
    workValues.filterMutex.unlock();
    ui->previewLabel->setText(tr("View: Low Res"));
}

void APhotoToolLibre::showFullResolutionImage()
{
    QImage tempImage = values.imageOriginal;
    showImage(tempImage);
    values.filteredImageWidth = values.image.width();
    values.filteredImageHeight = values.image.height();
    showFileInfo();
    workValues.filterMutex.lock();
    workValues.lastFullResTimestamp = TimeUtil::getTimestamp();
    workValues.filterMutex.unlock();
    ui->previewLabel->setText(tr("View: Full Res"));
}


void APhotoToolLibre::applySizingFiltersToGetNewSize()
{
    QImage tempImage = values.imageOriginal;
    tempImage = Filters::applySizingFilters(tempImage, values.filterValues);
    values.filteredImageWidth = tempImage.width();
    values.filteredImageHeight = tempImage.height();
    showFileInfo();
}

void APhotoToolLibre::showImage(const QImage &imageToShow) {
    values.image = Filters::applyFilters(imageToShow, values.filterValues);
    ui->scrollArea->setWidgetResizable(true);
    Graphics::fitImage(values.image, *ui->imageLabel);
}

void APhotoToolLibre::createFullResolutionInBackground() {
    if (values.imageOriginal.height() == 0) return;
    workValues.filterMutex.lock();
    ImageCreator::createFullResolutionInBackground(&workValues, &values);
    workValues.filterMutex.unlock();
}

void APhotoToolLibre::backgroundFilterReady() {
    workValues.filterMutex.lock();
    ImageCreator::backgroundFilterReady(&workValues, &values, ui);
    workValues.filterMutex.unlock();
}

bool APhotoToolLibre::event(QEvent *event)
{
    if (QEvent::None != ShowImageEvent::eventType && event->type() == ShowImageEvent::eventType) {
        applySizingFiltersToGetNewSize();
        showPreviewImage();
        ui->imageLabel->update();
        return true;
    }
    return QWidget::event(event);
}

void APhotoToolLibre::onPrintClicked() {
    Print print;
    print.print(values.image);
}

void APhotoToolLibre::showSettings() {
    OptionsDialog dialog;
    dialog.prepareSettingsDialog(this);
    dialog.setModal(true);
    dialog.exec();
}

void APhotoToolLibre::writeSettings()
{
    SettingsUtils::writeSettings(&appSettings, this);
}

void APhotoToolLibre::readSettings()
{
    SettingsUtils::readSettings(&appSettings, this);
}

UserSettingValues* APhotoToolLibre::getAppSettings()
{
    return &appSettings;
}


Ui::MainWindow *APhotoToolLibre::getUi() const
{
    return ui;
}

void APhotoToolLibre::about()
{
    QMessageBox::about(this, tr("About A Photo Tool (Libre) version ") + APTL_VERSION,
                       tr("<p>The <b>A Photo Tool (Libre)</b> is a photo editing application.</p>"
                          "<p>Copyright &copy; 2021-2022 Jari Ahola</p>"
                          "<p>Released under GNU General Public License version 3 (GPLv3)</p>"
                          "<p>See license text <a href=\"https://www.gnu.org/licenses/gpl-3.0-standalone.html\">here at gnu.org</a>.</p>"
                          "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, "
                          "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS "
                          "FOR A PARTICULAR PURPOSE.</p>"));
}



