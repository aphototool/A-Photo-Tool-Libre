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

#include "rotatetoolui.h"
#include "./ui_aphototoollibre.h"

RotateToolUi::RotateToolUi(QMainWindow *mainWin, Ui::MainWindow *ui, Values *values)
{
    this->mainWin = mainWin;
    this->ui = ui;
    this->values = values;

    ui->rotateFrame->setVisible(false);
    QObject::connect(ui->rotateButton, &QPushButton::clicked, this, &RotateToolUi::onRotateButtonClicked);
    QObject::connect(ui->rotateLeftButton, &QPushButton::clicked, this, &RotateToolUi::onRotateLeftButtonClicked);
    QObject::connect(ui->rotateRightButton, &QPushButton::clicked, this, &RotateToolUi::onRotateRightButtonClicked);
    QObject::connect(ui->rotateCancelButton, &QPushButton::clicked, this, &RotateToolUi::onRotateCancelButtonClicked);
    QObject::connect(ui->rotateOkButton, &QPushButton::clicked, this, &RotateToolUi::onRotateOkButtonClicked);
    ui->rotateFrame->installEventFilter(this);
}

void RotateToolUi::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    Graphics::fitImage(imageToRotate, *ui->imageLabel);
}

void RotateToolUi::onRotateButtonClicked()
{
    ui->cropFrame->setVisible(false);
    ui->resizeFrame->setVisible(false);
    QTimer::singleShot(250, this, [this]() { ui->rotateFrame->setVisible(!ui->rotateFrame->isVisible()); } );
}

void RotateToolUi::showRotateTool()
{
    imageToRotate = values->image.copy();
    newCropValues = values->filterValues.cropValues == nullptr ? nullptr : values->filterValues.cropValues->copy();
    angle = values->filterValues.rotateAngle;
    newFilteredImageWidth = values->filteredImageWidth;
    newFilteredImageHeight = values->filteredImageHeight;
    newResizeCalcType = values->filterValues.resizeType;
}

void RotateToolUi::onRotateLeftButtonClicked()
{
    rotatePreview(-90);
}

void RotateToolUi::onRotateRightButtonClicked()
{
    rotatePreview(90);
}

void RotateToolUi::rotateCrop(int angle)
{
    if (newCropValues == nullptr) return;

    CropValues *temp = newCropValues->copy();
    if (angle > 0) {
        newCropValues->x1 = 1.0 - temp->y2;
        newCropValues->y1 = temp->x1;
        newCropValues->x2 = 1.0 - temp->y1;
        newCropValues->y2 = temp->x2;
    } else {
        newCropValues->x1 = temp->y1;
        newCropValues->y1 = 1.0 - temp->x2;
        newCropValues->x2 = temp->y2;
        newCropValues->y2 = 1.0 - temp->x1;
    }
    if (newResizeCalcType == ResizeCalcType::Width) {
        newResizeCalcType = ResizeCalcType::Height;
    } else if (newResizeCalcType == ResizeCalcType::Height) {
        newResizeCalcType = ResizeCalcType::Width;
    }
    std::swap(newFilteredImageWidth, newFilteredImageHeight);
    if (newCropValues->x1 > newCropValues->x2) std::swap(newCropValues->x2, newCropValues->x1);
    if (newCropValues->y1 > newCropValues->y2) std::swap(newCropValues->y2, newCropValues->y1);
}

void RotateToolUi::rotatePreview(int angle)
{
    rotateCrop(angle);
    this->angle = (this->angle + angle) % 360;
    QTransform rotating;
    rotating.rotate(angle);
    imageToRotate = imageToRotate.transformed(rotating);
    Graphics::fitImage(imageToRotate, *ui->imageLabel);
}

bool RotateToolUi::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->rotateFrame && event->type() == QEvent::Hide) {
        qGuiApp->postEvent(mainWin, ShowImageEvent::getEvent());
    } else if (obj == ui->rotateFrame && event->type() == QEvent::Show) {
        showRotateTool();
    }
    return QObject::eventFilter(obj, event);
}

void RotateToolUi::onRotateCancelButtonClicked()
{
    ui->rotateFrame->setVisible(false);
}

void RotateToolUi::onRotateOkButtonClicked()
{
    values->filterValues.rotateAngle = angle;
    if (newCropValues != nullptr) {
        values->filterValues.cropValues = newCropValues->copy();
        values->filteredImageWidth = newFilteredImageWidth;
        values->filteredImageHeight = newFilteredImageHeight;
        values->filterValues.resizeType = newResizeCalcType;
        delete newCropValues;
        newCropValues = nullptr;
    }
    values->imageModified = true;
    ui->rotateFrame->setVisible(false);
}
