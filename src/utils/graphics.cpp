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
    imageLabel.setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
    imageLabel.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}
