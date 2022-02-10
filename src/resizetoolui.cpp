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

#include "resizetoolui.h"
#include "./ui_aphototoollibre.h"

ResizeToolUi::ResizeToolUi(QMainWindow *mainWin, Ui::MainWindow *ui, Values *values)
{
    this->mainWin = mainWin;
    this->ui = ui;
    this->values = values;

    ui->resizeFrame->setVisible(false);
    QObject::connect(ui->resizeButton, &QPushButton::clicked, this, &ResizeToolUi::onResizeButtonClicked);
    QObject::connect(ui->widthResizeValue, &QLineEdit::textChanged, this, &ResizeToolUi::onWidthChanged);
    QObject::connect(ui->heightResizeValue, &QLineEdit::textChanged, this, &ResizeToolUi::onHeightChanged);
    QObject::connect(ui->percentageResizeValue, &QLineEdit::textChanged, this, &ResizeToolUi::onPercentageChanged);
    QObject::connect(ui->resizeCancelButton, &QPushButton::clicked, this, &ResizeToolUi::onResizeCancelButtonClicked);
    QObject::connect(ui->resizeOkButton, &QPushButton::clicked, this, &ResizeToolUi::onResizeOkButtonClicked);
    ui->resizeFrame->installEventFilter(this);
}

void ResizeToolUi::onResizeButtonClicked()
{
    ui->resizeFrame->setVisible(!ui->resizeFrame->isVisible());
    ui->cropFrame->setVisible(false);
    ui->rotateFrame->setVisible(false);
}

void ResizeToolUi::onResizeCancelButtonClicked()
{
    ui->resizeFrame->setVisible(false);
}

void ResizeToolUi::onResizeOkButtonClicked()
{
    switch (resizeType) {
    case ResizeCalcType::Width:
        values->filterValues.resizeValue = newWidth;
        values->filterValues.resizeType = ResizeCalcType::Width;
        break;
    case ResizeCalcType::Height:
        values->filterValues.resizeValue = newHeight;
        values->filterValues.resizeType = ResizeCalcType::Height;
        break;
    case ResizeCalcType::Percentage:
        values->filterValues.resizeValue = newPercentage;
        values->filterValues.resizeType = ResizeCalcType::Percentage;
        break;
    default:
        break;
    }
    values->filterValues.resizePercentageValue = newPercentage;
    values->imageModified = true;
    ui->resizeFrame->setVisible(false);
}

void ResizeToolUi::onWidthChanged()
{
    int newValue = ui->widthResizeValue->text().toUInt();
    if (newValue > 0) {
        ui->widthResizeValue->setStyleSheet("");
        newWidth = newValue;
        calculateSize(ResizeCalcType::Width);
    } else {
        ui->widthResizeValue->setStyleSheet(lineEditErrorStyle);
    }
}

void ResizeToolUi::onHeightChanged()
{
    int newValue = ui->heightResizeValue->text().toUInt();
    if (newValue > 0) {
        ui->heightResizeValue->setStyleSheet("");
        newHeight = newValue;
        calculateSize(ResizeCalcType::Height);
    } else {
        ui->heightResizeValue->setStyleSheet(lineEditErrorStyle);
    }
}

void ResizeToolUi::onPercentageChanged()
{
    int newValue = ui->percentageResizeValue->text().toInt();
    if (newValue > 0) {
        ui->percentageResizeValue->setStyleSheet("");
        newPercentage = (double)newValue;
        calculateSize(ResizeCalcType::Percentage);
    } else {
        ui->percentageResizeValue->setStyleSheet(lineEditErrorStyle);
    }
}

bool ResizeToolUi::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->resizeFrame && event->type() == QEvent::Hide) {
        qGuiApp->postEvent(mainWin, ShowImageEvent::getEvent());
    } else if (obj == ui->resizeFrame && event->type() == QEvent::Show) {
        showResizeTool();
    }
    return QObject::eventFilter(obj, event);
}

void ResizeToolUi::showResizeTool()
{
    double scale = values->filterValues.resizePercentageValue / 100.0;
    if (scale <= 0.0) scale = 1.0;
    oldWidth = values->filteredImageWidth / scale;
    oldHeight = values->filteredImageHeight / scale;
    newWidth = values->filteredImageWidth;
    newHeight = values->filteredImageHeight;
    newPercentage = (int)values->filterValues.resizePercentageValue;
    ui->oldSizeLabel->setText(QString("%1 X %2")
                              .arg(oldWidth)
                              .arg(oldHeight));
    setFieldValues();
}

void ResizeToolUi::setFieldValues()
{
    ui->widthResizeValue->blockSignals(true);
    ui->widthResizeValue->setText(QString("%1").arg(newWidth));
    ui->widthResizeValue->blockSignals(false);

    ui->heightResizeValue->blockSignals(true);
    ui->heightResizeValue->setText(QString("%1").arg(newHeight));
    ui->heightResizeValue->blockSignals(false);

    ui->percentageResizeValue->blockSignals(true);
    ui->percentageResizeValue->setText(QString("%1").arg((int)newPercentage));
    ui->percentageResizeValue->blockSignals(false);

    ui->newSizeLabel->setText(QString("%1 X %2")
                              .arg(newWidth)
                              .arg(newHeight));
}

void ResizeToolUi::calculateSize(ResizeCalcType type)
{
    if (oldWidth <= 0 || oldHeight <= 0) return;

    double scale = 1.0;
    switch (type) {
    case ResizeCalcType::Width:
        scale = (double)newWidth / oldWidth;
        newHeight = (int)(oldHeight * scale);
        newPercentage = scale * 100;
        resizeType = ResizeCalcType::Width;
        break;
    case ResizeCalcType::Height:
        scale = (double)newHeight / oldHeight;
        newWidth = (int)(oldWidth * scale);
        newPercentage = scale * 100;
        resizeType = ResizeCalcType::Height;
        break;
    case ResizeCalcType::Percentage:
        scale = newPercentage / 100.0;
        newWidth = (int)(oldWidth * scale);
        newHeight = (int)(oldHeight * scale);
        resizeType = ResizeCalcType::Percentage;
        break;
    default:
        break;
    }
    setFieldValues();
}

