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

#include "imagelabelpainter.h"

ImageLabelPainter::ImageLabelPainter()
{

}

ImageLabelPainter::~ImageLabelPainter()
{

}

void ImageLabelPainter::paintOnLabel(QLabel *label, CropValues *cropValues) {

    int imageW = getPixmapWidth(label->pixmap(Qt::ReturnByValue));
    int imageH = getPixmapHeight(label->pixmap(Qt::ReturnByValue));
    int w = label->width();
    int h = label->height();
    int sectors = 5;

    QPainter *painter = new QPainter(label);
    paintGrid(painter, sectors, w, h);
    if (cropValues != nullptr && cropValues->useCrop) {
        paintCropHandles(painter,
                         (w - imageW) / 2 + cropValues->x1 * imageW,
                         (h - imageH) / 2 + cropValues->y1 * imageH,
                         (w - imageW) / 2 + cropValues->x2 * imageW,
                         (h - imageH) / 2 + cropValues->y2 * imageH);
    }
    delete painter;
}

void ImageLabelPainter::paintGrid(QPainter *painter, int sectors, int width, int height)
{
    int dW = width / sectors;
    int dH = height / sectors;
    painter->setBrush(Qt::NoBrush);
    QColor color(255, 255, 255, 125);
    QPen pen(color);
    pen.setWidth(2);
    painter->setPen(pen);
    for (int i = 1; i < sectors; i++)
    {
        painter->drawLine(0, i * dH, width, i * dH);
        painter->drawLine(i * dW, 0, i * dW, height);
    }
}

void ImageLabelPainter::paintCropHandles(QPainter *painter, int x1, int y1, int x2, int y2)
{
    const int corner = 10;
    painter->setBrush(Qt::NoBrush);
    QColor color(255, 0, 0, 255);
    QPen pen(color);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(x1, y1, x2, y1);
    painter->drawLine(x1, y1, x1, y2);
    painter->drawLine(x1, y2, x2, y2);
    painter->drawLine(x2, y1, x2, y2);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->drawLine(x1, y1, x1 + corner, y1);
    painter->drawLine(x1, y1, x1, y1 + corner);
    painter->drawLine(x2, y1, x2 - corner, y1);
    painter->drawLine(x2, y1, x2, y1 + corner);
    painter->drawLine(x1, y2, x1 + corner, y2);
    painter->drawLine(x1, y2, x1, y2 - corner);
    painter->drawLine(x2, y2, x2 - corner, y2);
    painter->drawLine(x2, y2, x2, y2 - corner);
}
