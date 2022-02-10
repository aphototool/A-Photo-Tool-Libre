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


#include "contrastfilter.h"

ContrastFilter::ContrastFilter()
{

}

QImage ContrastFilter::adjustContrast(const QImage &image, float contrastDelta)
{
    QImage newImage = image.copy();

    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    float factor = (259.0 * (contrastDelta + 255.0)) / (255.0 * (259.0 - contrastDelta));
    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            red = factor * float(qRed(line[x]) - 128) + 128.0;
            green = factor * float(qGreen(line[x]) - 128) + 128.0;
            blue = factor * float(qBlue(line[x]) - 128) + 128.0;
            newImage.setPixel(x,y, qRgb(FilterValues::toValidValue(red), FilterValues::toValidValue(green), FilterValues::toValidValue(blue)));
        }
    }
    return newImage;
}
