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

#include "croptoolui.h"
#include "./ui_aphototoollibre.h"

CropToolUi::CropToolUi(QMainWindow *mainWin, Ui::MainWindow *ui, Values *values, WorkValues *workValues)
{
    this->mainWin = mainWin;
    this->ui = ui;
    this->values = values;
    this->workValues = workValues;

    ui->cropFrame->setVisible(false);
    QObject::connect(ui->cropButton, &QPushButton::clicked, this, &CropToolUi::onCropButtonClicked);
    QObject::connect(ui->cropOkButton, &QPushButton::clicked, this, &CropToolUi::onCropOkButtonClicked);
    QObject::connect(ui->cropCancelButton, &QPushButton::clicked, this, &CropToolUi::onCropCancelButtonClicked);
    QObject::connect(ui->straightenSlider, &QSlider::valueChanged, this, &CropToolUi::onStraightenSliderValueChanged);
    QObject::connect(ui->useCropCheckBox, &QCheckBox::stateChanged, this, &CropToolUi::onUseCropCheckBoxClicked);
    ui->cropFrame->installEventFilter(this);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QObject::connect(ui->cropFormatButtonGroup, &QButtonGroup::buttonClicked, this, static_cast<void (CropToolUi::*)()>(&CropToolUi::useCropFormat));
#else
    QObject::connect(ui->cropFormatButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [=](int id) {
            emit useCropFormat(id);
        });
#endif
    ui->cropFree->setChecked(true);
    ui->cropFormatButtonGroup->setId(ui->cropFree, (int)CropFormat::_Free);
    ui->cropFormatButtonGroup->setId(ui->crop1to1, (int)CropFormat::_1x1);
    ui->cropFormatButtonGroup->setId(ui->crop4to3, (int)CropFormat::_4x3);
    ui->cropFormatButtonGroup->setId(ui->crop3to2, (int)CropFormat::_3x2);
    ui->cropFormatButtonGroup->setId(ui->crop16to9, (int)CropFormat::_16x9);
    ui->cropFormatButtonGroup->setId(ui->cropLocked, (int)CropFormat::_Locked);
}

void CropToolUi::onCropButtonClicked()
{
    ui->rotateFrame->setVisible(false);
    ui->resizeFrame->setVisible(false);
    QTimer::singleShot(250, this, [this]() { ui->cropFrame->setVisible(!ui->cropFrame->isVisible()); } );
}

void CropToolUi::showCropTool() {
    BackgroundControl::haltBackgroundWork(values, workValues);

    imageToRotate = values->imageOriginalScaled.copy();

    if (values->filterValues.cropValues != nullptr) {
        newCropValues = values->filterValues.cropValues->copy();
    } else {
        newCropValues = new CropValues();
        newCropValues->x1 = 0;
        newCropValues->y1 = 0;
        newCropValues->x2 = 1.0;
        newCropValues->y2 = 1.0;
        newCropValues->useCrop = true;
    }
    ui->imageLabel->cropValues = newCropValues;
    ui->useCropCheckBox->setCheckState(newCropValues->useCrop ? Qt::Checked : Qt::Unchecked);
    ui->cropFormatGroupBox->setEnabled(newCropValues->useCrop);
    ui->cropFree->setChecked(true);
    cropAreaValidator = new CropAreaValidator(ui->imageLabel, ui->cropLocked, newCropValues);
    useCropFormat();
    cropAreaValidator->calculateCropFormatMultiplier();

    FilterValues* tempValues = values->filterValues.copy();
    tempValues->straightenAngle = 0; // Omit saved straihgten value
    tempValues->cropValues = nullptr; // Omit saved crop
    tempValues->resizeType = ResizeCalcType::None; // Omit resize
    imageToRotate = Filters::applyFilters(imageToRotate, *tempValues);
    angle = values->filterValues.straightenAngle;
    if (angle >= -45 && angle <= 45) {
        ui->straightenSlider->setValue(angle);
    } else {
        ui->straightenSlider->setValue(0);
    }
    rotatePreview(angle);
    ui->imageLabel->imageLabelPainter = new ImageLabelPainter();
    ui->imageLabel->update();
    needsPostShowImageEvent = true;
    delete tempValues;
}

void CropToolUi::onStraightenSliderValueChanged(int value)
{
    rotatePreview(value);
    useCropFormat();
}

void CropToolUi::rotatePreview(int angle)
{
    this->angle = angle;
    int newAngle = this->angle;
    QTransform rotating;
    rotating.rotate(newAngle);
    QImage newTempImage = imageToRotate.transformed(rotating);
    ui->scrollArea->setWidgetResizable(true);
    Graphics::fitImage(newTempImage, *ui->imageLabel);
}

void CropToolUi::mousePressEvent(QMouseEvent *event)
{
    if (!ui->cropFrame->isVisible()) return;

    imageXStart = (ui->imageLabel->width() - getPixmapWidth(ui->imageLabel->pixmap())) / 2;
    imageXEnd = cropXStart + getPixmapWidth(ui->imageLabel->pixmap());
    imageYStart = (ui->imageLabel->height() - getPixmapHeight(ui->imageLabel->pixmap())) / 2;
    imageYEnd = cropYStart + getPixmapHeight(ui->imageLabel->pixmap());

    cropXStart = (ui->imageLabel->width() - getPixmapWidth(ui->imageLabel->pixmap())) / 2 + newCropValues->x1 * getPixmapWidth(ui->imageLabel->pixmap());
    cropXEnd = cropXStart + (newCropValues->x2 - newCropValues->x1) * getPixmapWidth(ui->imageLabel->pixmap());
    cropYStart = (ui->imageLabel->height() - getPixmapHeight(ui->imageLabel->pixmap())) / 2 + newCropValues->y1 * getPixmapHeight(ui->imageLabel->pixmap());
    cropYEnd = cropYStart + (newCropValues->y2 - newCropValues->y1)  * getPixmapHeight(ui->imageLabel->pixmap());

    mousePresPos = event->pos();
    int xMouse = ui->imageLabel->mapFrom(mainWin, event->pos()).x();
    int yMouse = ui->imageLabel->mapFrom(mainWin, event->pos()).y();

    if (abs(cropXStart - xMouse) <= clickTolerance && abs(cropYStart - yMouse) <= clickTolerance) {
        cropCorner = CropCorner::TopLeft;
    } else if (abs(cropXEnd - xMouse) <= clickTolerance && abs(cropYStart - yMouse) <= clickTolerance) {
        cropCorner = CropCorner::TopRight;
    } else if (abs(cropXStart - xMouse) <= clickTolerance && abs(cropYEnd - yMouse) <= clickTolerance) {
        cropCorner = CropCorner::BottomLeft;
    } else if (abs(cropXEnd - xMouse) <= clickTolerance && abs(cropYEnd - yMouse) <= clickTolerance) {
        cropCorner = CropCorner::BottomRight;
    } else if (cropXStart + clickTolerance < xMouse && cropXEnd - clickTolerance > xMouse && cropYStart + clickTolerance < yMouse && cropYEnd - clickTolerance > yMouse) {
        cropCorner = CropCorner::InsideMove;
    } else if (abs(cropXStart - xMouse) <= clickTolerance && cropYStart < yMouse && cropYEnd > yMouse) {
        cropCorner = CropCorner::Left;
    } else if (abs(cropXEnd - xMouse) <= clickTolerance && cropYStart < yMouse && cropYEnd > yMouse) {
        cropCorner = CropCorner::Right;
    } else if (abs(cropYStart - yMouse) <= clickTolerance && cropXStart < xMouse && cropXEnd > xMouse) {
        cropCorner = CropCorner::Top;
    } else if (abs(cropYEnd - yMouse) <= clickTolerance && cropXStart < xMouse && cropXEnd > xMouse) {
        cropCorner = CropCorner::Bottom;
    } else {
        cropCorner = CropCorner::None;
    }
}

void CropToolUi::mouseMoveEvent(QMouseEvent *event)
{
    if (!ui->cropFrame->isVisible()) return;
    if (cropCorner == CropCorner::None) return;

    int xMouse = ui->imageLabel->mapFrom(mainWin, event->pos()).x();
    int yMouse = ui->imageLabel->mapFrom(mainWin, event->pos()).y();

    double width = (double)getPixmapWidth(ui->imageLabel->pixmap());
    double height = (double)getPixmapHeight(ui->imageLabel->pixmap());

    switch (cropCorner) {
        case CropCorner::TopLeft:
            newCropValues->x1 = (xMouse - imageXStart) / width;
            newCropValues->y1 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::TopRight:
            newCropValues->x2 = (xMouse - imageXStart) / width;
            newCropValues->y1 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::BottomLeft:
            newCropValues->x1 = (xMouse - imageXStart) / width;
            newCropValues->y2 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::BottomRight:
            newCropValues->x2 = (xMouse - imageXStart) / width;
            newCropValues->y2 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::Left:
            newCropValues->x1 = (xMouse - imageXStart) / width;
            break;
        case CropCorner::Right:
            newCropValues->x2 = (xMouse - imageXStart) / width;
            break;
        case CropCorner::Top:
            newCropValues->y1 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::Bottom:
            newCropValues->y2 = (yMouse - imageYStart) / height;
            break;
        case CropCorner::InsideMove:
            {
                int dx = event->pos().x() - mousePresPos.x();
                newCropValues->x1 = (cropXStart - imageXStart + dx) / width;
                newCropValues->x2 = (cropXEnd - imageXStart + dx) / width;
                int dy = event->pos().y() - mousePresPos.y();
                newCropValues->y1 = (cropYStart - imageYStart + dy) / height;
                newCropValues->y2 = (cropYEnd - imageYStart + dy) / height;
                break;
            }
        case CropCorner::None:
            break;
    }
    cropAreaValidator->validateCropArea(cropFormat, cropCorner);
    ui->imageLabel->update();
}

void CropToolUi::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (!ui->cropFrame->isVisible()) return;
    if (cropCorner == CropCorner::None) return;
    cropCorner = CropCorner::None;
}

void CropToolUi::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    rotatePreview(angle);
}

void CropToolUi::onCropCancelButtonClicked()
{
    ui->cropFrame->setVisible(false);
}

void CropToolUi::onCropOkButtonClicked()
{
    values->filterValues.straightenAngle = angle;
    values->filterValues.cropValues = newCropValues == nullptr ? nullptr : newCropValues->copy();
    values->imageModified = true;
    ui->cropFrame->setVisible(false);
}

void CropToolUi::onUseCropCheckBoxClicked(int state)
{
    newCropValues->useCrop = (bool)state;
    ui->cropFormatGroupBox->setEnabled(newCropValues->useCrop);
    ui->imageLabel->update();
}

void CropToolUi::useCropFormat(int)
{
    useCropFormat();
}

void CropToolUi::useCropFormat()
{
    cropFormat = static_cast<CropFormat>(ui->cropFormatButtonGroup->checkedId());
    if (cropFormat == CropFormat::_Locked) {
        cropAreaValidator->calculateCropFormatMultiplier();
    }
    cropAreaValidator->validateCropArea(cropFormat, cropCorner);
    ui->imageLabel->update();
}

bool CropToolUi::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->cropFrame && event->type() == QEvent::Hide) {
        hideCropTool();
        if (needsPostShowImageEvent) {
            needsPostShowImageEvent = false;
            qGuiApp->postEvent(mainWin, ShowImageEvent::getEvent());
        }
    } else if (obj == ui->cropFrame && event->type() == QEvent::Show) {
        showCropTool();
    }
    return QObject::eventFilter(obj, event);
}

void CropToolUi::hideCropTool()
{
    delete newCropValues;
    newCropValues = nullptr;
    delete ui->imageLabel->imageLabelPainter;
    ui->imageLabel->imageLabelPainter = nullptr;
    BackgroundControl::resumeBackgroundWork(values, workValues);
}
