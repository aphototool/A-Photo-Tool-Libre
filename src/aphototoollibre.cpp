/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2025 Jari Ahola
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
#include "qobjectdefs.h"

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

    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(onClipboardChange()));
    QObject::connect(ui->actionCopy, &QAction::triggered, this, &APhotoToolLibre::onClippoardCopy);
    QObject::connect(ui->actionPaste, &QAction::triggered, this, &APhotoToolLibre::onClippoardPaste);

    QObject::connect(ui->resetExposureButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetExposureClicked);
    QObject::connect(ui->resetColorsButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetColorsClicked);
    QObject::connect(ui->brightnessSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onBrightnessSliderValueChanged);
    QObject::connect(ui->contrastSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onContrastSliderValueChanged);
    QObject::connect(ui->lightsSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onLightsSliderValueChanged);
    QObject::connect(ui->darksSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onDarksSliderValueChanged);
    QObject::connect(ui->saturationSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onSaturationSliderValueChanged);
    QObject::connect(ui->hueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onHueSliderValueChanged);
    QObject::connect(ui->colorTemperatureSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onColorTemperatureSliderValueChanged);
    QObject::connect(ui->tintSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onTintSliderValueChanged);
    QObject::connect(ui->naturalButton, &QPushButton::clicked, this, &APhotoToolLibre::onNaturalButtonClicked);
    QObject::connect(ui->panchromaticButton, &QPushButton::clicked, this, &APhotoToolLibre::onPanchromaticButtonClicked);
    QObject::connect(ui->enableBWCheckBox, &QCheckBox::checkStateChanged, this, &APhotoToolLibre::onEnableBWCheckBoxStateChanged);
    QObject::connect(ui->redSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onRedSliderValueChanged);
    QObject::connect(ui->greenSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onGreenSliderValueChanged);
    QObject::connect(ui->blueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onBlueSliderValueChanged);
    QObject::connect(ui->actionOptions, &QAction::triggered, this, &APhotoToolLibre::showSettings);
    QObject::connect(&fullResolutionWorkValues.filterWatcher, &QFutureWatcherBase::finished, this, &APhotoToolLibre::backgroundFilterReady);
    QObject::connect(&histogramWorkValues.filterWatcher, &QFutureWatcherBase::finished, this, &APhotoToolLibre::backgroundHistogramReady);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &APhotoToolLibre::about);
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
    helpMenu->addSeparator();
    helpMenu->addAction(tr("Check for Updates..."), this, &APhotoToolLibre::checkForUpdates);

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    ui->actionPrint->setEnabled(true);
#else
    ui->actionPrint->setEnabled(false);
#endif

    rotateToolUi = new RotateToolUi(this, ui, &values, &fullResolutionWorkValues);
    cropToolUi = new CropToolUi(this, ui, &values, &fullResolutionWorkValues);
    resizeToolUi = new ResizeToolUi(this, ui, &values);

    ui->mainToolsWidget->setCurrentIndex(0);

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(false);
    ui->imageLabel->setLoadImageFunction(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    resetValues();

    readSettings();
    if (appSettings.getDarkMode()) {
        StyleMode::darkMode(qApp);
        ui->redSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("red", "#232323"));
        ui->greenSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("green", "#232323"));
        ui->blueSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("blue", "#232323"));
        ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : #232323; }");
    } else {
        StyleMode::lightMode(qApp);
        ui->redSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("red", "lightGray"));
        ui->greenSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("green", "lightGray"));
        ui->blueSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("blue", "lightGray"));
        ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : lightGray; }");
    }

    showHistogram(appSettings.getShowHistogram());

    if (appSettings.getFirstUse()) {
        QImage *welcomeImage = new QImage(":/resources/Welcome.png");
        QTimer::singleShot(250, this, [welcomeImage, this]() { showImage(*welcomeImage); } );
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

    if (getenv("APPIMAGE")) {
        int size = 64; // Nearly good taskbar icon and just acceptable window icon
        QPixmap pixmap = QPixmap(":/resources/aphototoollibre.svg")
                .scaledToHeight(size, Qt::SmoothTransformation)
                .scaledToWidth(size, Qt::SmoothTransformation);
        qApp->setWindowIcon(QIcon(pixmap));
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
    onClipboardChange();
    ui->histogramTextLabel->setText(tr("Histogram *"));
    histogramWorkValues.lastWorkTimestamp = 0;
}

void APhotoToolLibre::useDarkMode(bool darkMode) {
    if (darkMode) {
        StyleMode::darkMode(qApp);
        ui->redSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("red", "#232323"));
        ui->greenSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("green", "#232323"));
        ui->blueSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("blue", "#232323"));
        ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : #232323; }");
    } else {
        StyleMode::lightMode(qApp);
        ui->redSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("red", "lightGray"));
        ui->greenSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("green", "lightGray"));
        ui->blueSlider->setStyleSheet(StyleMode::getStyleSheetForColorSlider("blue", "lightGray"));
        ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : lightGray; }");
    }

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
    onClipboardChange();
}

void APhotoToolLibre::onSaveButtonClicked()
{
    if (values.image.height() <= 0) {
        if (QMessageBox::Close == QMessageBox(QMessageBox::Information, tr("Nothing to save"), tr("Load new photo to edit and then save."), QMessageBox::Close, this).exec())
        {
            return;
        }
    }

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
    values.filterValues.tintValue = 0.0;
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

void APhotoToolLibre::onTintSliderValueChanged(int value)
{
    values.filterValues.tintValue = float(value);
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
    ui->tintSlider->setValue(values.filterValues.tintValue);
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
    fullResolutionWorkValues.filterMutex.lock();
    values.filteredTime = TimeUtil::getTimestamp();
    fullResolutionWorkValues.filterMutex.unlock();
    ui->previewLabel->setText(tr("View: Low Res"));
    ui->histogramTextLabel->setText(tr("Histogram *"));
}

void APhotoToolLibre::showFullResolutionImage()
{
    QImage tempImage = values.imageOriginal;
    showImage(tempImage);
    values.filteredImageWidth = values.image.width();
    values.filteredImageHeight = values.image.height();
    showFileInfo();
    fullResolutionWorkValues.filterMutex.lock();
    fullResolutionWorkValues.lastWorkTimestamp = TimeUtil::getTimestamp();
    fullResolutionWorkValues.filterMutex.unlock();
    ui->previewLabel->setText(tr("View: Full Res"));
    createHistogramInBackground();
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
    fullResolutionWorkValues.filterMutex.lock();
    ImageCreator::createFullResolutionInBackground(&fullResolutionWorkValues, &values);
    fullResolutionWorkValues.filterMutex.unlock();
}

void APhotoToolLibre::backgroundFilterReady() {
    fullResolutionWorkValues.filterMutex.lock();
    bool ready = ImageCreator::backgroundFilterReady(&fullResolutionWorkValues, &values);
    if (ready) {
        ui->scrollArea->setWidgetResizable(true);
        Graphics::fitImage(values.image, *ui->imageLabel);
        ui->previewLabel->setText(QCoreApplication::translate("APhotoToolLibre", "View: Full Res"));
    }
    fullResolutionWorkValues.filterMutex.unlock();
    createHistogramInBackground();
}

void APhotoToolLibre::createHistogramInBackground() {
    if (values.imageOriginal.height() == 0) return;
    histogramWorkValues.filterMutex.lock();
    Histogram::createHistogramInBackground(&histogramWorkValues, &values);
    histogramWorkValues.filterMutex.unlock();
}

void APhotoToolLibre::backgroundHistogramReady() {
    histogramWorkValues.filterMutex.lock();
    QImage newImage = Histogram::backgroundHistogramReady(&histogramWorkValues, &values);
    if (!newImage.isNull()) {
        Graphics::fitImage(newImage, *ui->histogramLabel);
    }
    ui->histogramTextLabel->setText(tr("Histogram"));
    histogramWorkValues.filterMutex.unlock();
}

void APhotoToolLibre::showHistogram(bool show) {
    ui->histogramLabel->setHidden(!show);
    ui->histogramTextLabel->setHidden(!show);
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
    if (values.image.height() <= 0) {
        if (QMessageBox::Close == QMessageBox(QMessageBox::Information, tr("Nothing to print"), tr("Load new photo to edit and print."), QMessageBox::Close, this).exec())
        {
            return;
        }
    }
    Print print;
    print.print(values.image);
}

void APhotoToolLibre::onClipboardChange() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    ui->actionPaste->setEnabled(mimeData->hasImage() || mimeData->hasUrls());
    ui->actionCopy->setEnabled(values.image.height() > 0);
}

void APhotoToolLibre::onClippoardCopy() {
    if (values.image.height() <= 0) {
        if (QMessageBox::Close == QMessageBox(QMessageBox::Information, tr("Nothing to copy"), tr("Load new photo to edit and copy."), QMessageBox::Close, this).exec())
        {
            return;
        }
    }
    QClipboard *clipboard = QApplication::clipboard();
    QPixmap pixmap = QPixmap::fromImage(values.image);
    clipboard->setPixmap(pixmap, QClipboard::Clipboard);
}

void APhotoToolLibre::onClippoardPaste() {

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasUrls()) {
        QUrl url = mimeData->urls().constFirst();
        if (url.isLocalFile()) {
            loadImage(url.toLocalFile());
            return;
        }
    }
    if (mimeData->hasImage()) {
        const QPixmap pixmap = qvariant_cast<QPixmap>(mimeData->imageData());
        QImage tempImage = pixmap.toImage();
        if (tempImage.format() == QImage::Format_Invalid) return;
        if (isLoadNewPhtoOk()) {
            loadImage("image_copy", tempImage);
        }
    }

}

void APhotoToolLibre::onDropImage(QImage droppedImage) {
    if (isLoadNewPhtoOk()) {
        if (droppedImage.format() == QImage::Format_Invalid) return;
        loadImage("image_copy", droppedImage);
    }
}

void APhotoToolLibre::checkForUpdates() {
    QString path = qApp->applicationDirPath();
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setWindowTitle(tr("Check for Updates"));
    QString text = "";
    if (path.contains("/snap/")) {
        text.append("Looks like you are running Snap release. ");
        text.append("Snaps are normally updated automatically.");
        text.append("<br /><br />");
    } else if (getenv("APPIMAGE")) {
        text.append("Looks like you are running AppImage release. ");
        text.append("New versions are released on GitHub.");
        text.append("<br /><br />");
    } else if (getenv("container")) {
        text.append("Looks like you are running FlatPak release. ");
        text.append("Use your normal App Store or FaltPak tool to update.");
        text.append("<br /><br />");
    }
    text.append("You are currenty running version <b>");
    text.append(APTL_VERSION);
    text.append("</b> of A Photo Tool (Libre).");
    text.append("<br /><br />");
    text.append("You can check latest released version at GitHub.");
    text.append("<br /><br />");
    text.append("<a href='https://github.com/aphototool/A-Photo-Tool-Libre/releases'>https://github.com/aphototool/A-Photo-Tool-Libre/releases</a>");
    text.append("<br />");
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    if (msgBox.exec()) {
        return;
    }
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
    QMessageBox::about(this, tr("About A Photo Tool (Libre)"),
                       tr("<p>The <b>A Photo Tool (Libre)</b> is a photo editing application.</p>"
                          "<p>Version "
                          APTL_VERSION
                          "</p>"
                          "<p>Copyright &copy; 2021-2025 Jari Ahola</p>"
                          "<p>Released under GNU General Public License version 3 (GPLv3)</p>"
                          "<p>See license text <a href=\"https://www.gnu.org/licenses/gpl-3.0-standalone.html\">here at gnu.org</a>.</p>"
                          "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, "
                          "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS "
                          "FOR A PARTICULAR PURPOSE.</p>"));
}



