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
    QObject::connect(ui->showFullResolutionButton, &QPushButton::clicked, this, &APhotoToolLibre::onShowFullResolutionClicked);
    QObject::connect(ui->resetExposureButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetExposureClicked);
    QObject::connect(ui->resetColorsButton, &QPushButton::clicked, this, &APhotoToolLibre::onResetColorsClicked);
    QObject::connect(ui->lightnessSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onLightnessSliderValueChanged);
    QObject::connect(ui->contrastSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onContrastSliderValueChanged);
    QObject::connect(ui->saturationSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onSaturationSliderValueChanged);
    QObject::connect(ui->hueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onHueSliderValueChanged);
    QObject::connect(ui->colorTemperatureSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onColorTemperatureSliderValueChanged);
    QObject::connect(ui->naturalButton, &QPushButton::clicked, this, &APhotoToolLibre::onNaturalButtonClicked);
    QObject::connect(ui->panchromaticButton, &QPushButton::clicked, this, &APhotoToolLibre::onPanchromaticButtonClicked);
    QObject::connect(ui->enableBWCheckBox, &QCheckBox::stateChanged, this, &APhotoToolLibre::onEnableBWCheckBoxStateChanged);
    QObject::connect(ui->redSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onRedSliderValueChanged);
    QObject::connect(ui->greenSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onGreenSliderValueChanged);
    QObject::connect(ui->blueSlider, &QSlider::valueChanged, this, &APhotoToolLibre::onBlueSliderValueChanged);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &APhotoToolLibre::about);
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    ui->actionPrint->setEnabled(true);
#else
    ui->actionPrint->setEnabled(false);
#endif

    qDebug() << ui->actionPrint->isEnabled();
    rotateToolUi = new RotateToolUi(this, ui, &values);
    cropToolUi = new CropToolUi(this, ui, &values);
    resizeToolUi = new ResizeToolUi(this, ui, &values);

    ui->mainToolsWidget->setCurrentIndex(0);

    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(false);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    resetValues();

    ui->redSlider->setStyleSheet(getStyleSheetForColorSlider("red"));
    ui->greenSlider->setStyleSheet(getStyleSheetForColorSlider("green"));
    ui->blueSlider->setStyleSheet(getStyleSheetForColorSlider("blue"));

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
}

void APhotoToolLibre::closeEvent(QCloseEvent *event)
{
    if (values.image.height() > 0 && values.imageModified) {
        if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Close Window?", "Close window and discard unsaved edits.", QMessageBox::Yes|QMessageBox::No).exec())
        {
            event->ignore();
            return;
        }
    }
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
    values.imageModified = false;
    setBWSliders();
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
    qDebug() << ui->actionPrint->isEnabled();
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
        if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Replace Image?", "Load new photo and discard unsaved edits.", QMessageBox::Yes|QMessageBox::No).exec())
        {
            return false;
        }
    }
    return true;
}

void APhotoToolLibre::loadImage(QString fileName, QImage tempImage) {
    resetValues();
    createPreviewImage(tempImage);
    showFullResolutionImage();
    values.originaFileName = fileName;
    showFileInfo();
}

void APhotoToolLibre::createPreviewImage(const QImage &tempImage) {
    values.imageOriginal = tempImage;
    if (values.imageOriginal.height() > PREVIEW_IMAGE_SIZE_LIMIT
            || values.imageOriginal.width() > PREVIEW_IMAGE_SIZE_LIMIT) {
        if (values.imageOriginal.height() > values.imageOriginal.width()) {
            values.imageOriginalScaled = values.imageOriginal
                    .scaledToHeight(PREVIEW_IMAGE_SIZE_LIMIT, Qt::SmoothTransformation);
        } else {
            values.imageOriginalScaled = values.imageOriginal
                    .scaledToWidth(PREVIEW_IMAGE_SIZE_LIMIT, Qt::SmoothTransformation);
        }
    } else {
        values.imageOriginalScaled = values.imageOriginal.copy();
    }
    values.image = values.imageOriginalScaled.copy();
}

void APhotoToolLibre::onSaveButtonClicked()
{
    showFullResolutionImage();
    SaveFile file;
    file.saveAs(&values);
}

void APhotoToolLibre::onShowFullResolutionClicked()
{
    showFullResolutionImage();
}

void APhotoToolLibre::onResetExposureClicked()
{
    values.filterValues.lightness = 0.0;
    values.filterValues.contrast = 0.0;
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

void APhotoToolLibre::onLightnessSliderValueChanged(int value)
{
    values.filterValues.lightness = float(value);
    values.imageModified = true;
    showPreviewImage();
}

void APhotoToolLibre::onContrastSliderValueChanged(int value)
{
    values.filterValues.contrast = float(value);
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
    ui->lightnessSlider->setValue(values.filterValues.lightness);
    ui->contrastSlider->setValue(values.filterValues.contrast);
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
    QString infoLine = QString("%1 %2 X %3")
            .arg(values.originaFileName)
            .arg(values.imageOriginal.width())
            .arg(values.imageOriginal.height());
    int width = values.filteredImageWidth;
    int height = values.filteredImageHeight;
    if (width != values.imageOriginal.width() || height != values.imageOriginal.height()) {
        infoLine.append(QString(" - new size %1 X %2").arg(width).arg(height));
    }
    ui->infoLabel->setText(infoLine);
}

void APhotoToolLibre::showPreviewImage()
{
    QImage tempImage = values.imageOriginalScaled;
    showImage(tempImage);
}

void APhotoToolLibre::showFullResolutionImage()
{
    QImage tempImage = values.imageOriginal;
    showImage(tempImage);
    values.filteredImageWidth = values.image.width();
    values.filteredImageHeight = values.image.height();
    showFileInfo();
}

void APhotoToolLibre::showImage(const QImage &imageToShow) {
    values.image = Filters::applyFilters(imageToShow, values.filterValues);
    ui->scrollArea->setWidgetResizable(true);
    Graphics::fitImage(values.image, *ui->imageLabel);
}

bool APhotoToolLibre::event(QEvent *event)
{
    if (QEvent::None != ShowImageEvent::eventType && event->type() == ShowImageEvent::eventType) {
        showFullResolutionImage();
        ui->imageLabel->update();
        return true;
    }
    return QWidget::event(event);
}

void APhotoToolLibre::onPrintClicked() {
    Print print;
    print.print(values.image);
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

QString APhotoToolLibre::getStyleSheetForColorSlider(const QString color) {
    return QString("QSlider::groove:vertical { background: black; position: absolute; left: 6px; right: 6px; width: 4px } "
                   "QSlider::handle:vertical { height: 14px; background: %1; margin: 0 -6px; border: 1px solid gray; border-radius: 8px;} "
                   "QSlider::add-page:vertical { background: %1; } "
                   "QSlider::sub-page:vertical { background: white; }").arg(color);
}



