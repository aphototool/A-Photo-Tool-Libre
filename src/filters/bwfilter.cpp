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

#include "bwfilter.h"

BWFilter::BWFilter()
{

}

QImage BWFilter::toBAndW(const QImage &image) {
    QImage newImage = image.copy();
    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            int average = (qRed(line[x]) + qGreen(line[x]) + qBlue(line[x]))/3;
            newImage.setPixel(x,y, qRgb(average, average, average));
        }
    }
    return newImage;
}

QImage BWFilter::toBAndW(const QImage &image, float rd, float gd, float bd) {
    QImage newImage = image.copy();
    float total = rd + gd + bd;
    if (total == 0) {
        return newImage;
    }

    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            float tempValue = float(qRed(line[x])) * rd + float(qGreen(line[x])) * gd + float(qBlue(line[x])) * bd;
            tempValue = tempValue / total;
            int newValue = 0;
            if (tempValue > 255.0) {
                newValue = 255;
            } else if (tempValue < 0.0) {
                newValue = 0;
            } else {
                newValue = int(tempValue);
            }
            newImage.setPixel(x,y, qRgb(newValue, newValue, newValue));
        }
    }
    return newImage;
}

