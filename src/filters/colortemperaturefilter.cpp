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

#include "colortemperaturefilter.h"

ColorTemperatureFilter::ColorTemperatureFilter()
{

}

/*
 * This filter function mimicks color temperature change to cooler or warmer colors.
 */
QImage ColorTemperatureFilter::changeColorTemperature(const QImage &image, float colorTemperature)
{
    QImage newImage = image.copy();

    if (colorTemperature == 0.0) return newImage;

    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            if (colorTemperature > 0.0) {
                red = qRed(line[x]) + colorTemperature;
                green = qGreen(line[x]) + colorTemperature / 2.0;
                blue = qBlue(line[x]);
            } else {
                red = qRed(line[x]);
                green = qGreen(line[x]) - colorTemperature / 2.0;
                blue = qBlue(line[x]) - colorTemperature;
            }
            newImage.setPixel(x,y, qRgb(FilterValues::toValidValue(red), FilterValues::toValidValue(green), FilterValues::toValidValue(blue)));
        }
    }
    return newImage;
}
