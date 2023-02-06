/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2023 Jari Ahola
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

#include "tintfilter.h"

TintFilter::TintFilter()
{

}


/*
 * This filter function mimicks tint change to green or magenta.
 */
QImage TintFilter::changeTint(const QImage &image, float tintValue)
{
    QImage newImage = image.copy();

    if (tintValue == 0.0) return newImage;

    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            if (tintValue > 0.0) {
                red = qRed(line[x]) + tintValue;
                green = qGreen(line[x]);
                blue = qBlue(line[x]) + tintValue / 2.0;
            } else {
                red = qRed(line[x]) - tintValue / 2.0;
                green = qGreen(line[x]) - tintValue;
                blue = qBlue(line[x]);
            }
            newImage.setPixel(x,y, qRgb(FilterValues::toValidValue(red), FilterValues::toValidValue(green), FilterValues::toValidValue(blue)));
        }
    }
    return newImage;
}
