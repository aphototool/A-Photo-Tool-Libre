/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2024 Jari Ahola
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

#include "graphics.h"

Graphics::Graphics()
{

}

QImage Graphics::drawGrid(const QImage &imageToDrawOn, const int sectors)
{
    QImage tempImage = imageToDrawOn;
    int w = tempImage.width();
    int h = tempImage.height();
    int dW = w / sectors;
    int dH = h / sectors;
    QPainter painter(&tempImage);
    painter.setBrush(Qt::NoBrush);
    QColor color(255, 255, 255, 125);
    QPen pen(color);
    pen.setWidth(4);
    painter.setPen(pen);
    for (int i = 1; i < sectors; i++)
    {
        painter.drawLine(0, i * dH, w, i * dH);
        painter.drawLine(i * dW, 0, i * dW, h);
    }
    return tempImage;
}

void Graphics::fitImage(const QImage &imageToFit, QLabel &imageLabel) {
    if (imageToFit.width() <= 0) return;
    QPixmap p = QPixmap::fromImage(imageToFit);
    int w = imageLabel.width();
    int h = imageLabel.height();
    imageLabel.setPixmap(p.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    imageLabel.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void Graphics::showFileInfo(Values values, QLabel *infoLabel, QMainWindow *window)
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
    infoLabel->setText(infoLine);
    if (values.originaFileName.length() > 0) {
        QString fileName = values.originaFileName.split("/").last();
        window->setWindowTitle("A Photo Tool (Libre) - " + fileName);
    } else {
        window->setWindowTitle("A Photo Tool (Libre)");
    }
}

