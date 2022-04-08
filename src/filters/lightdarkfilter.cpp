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

#include "lightdarkfilter.h"

LightDarkFilter::LightDarkFilter()
{

}

QImage LightDarkFilter::adjustLighsDarks(const QImage &image, float lightsDelta, float darksDelta) {
    QImage newImage = image.copy();
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    float brightness = 0.0;
    float deltaBrightness = 0.0;
    const float lightsTreshold = 150;
    const float darksTreshold = 130;
    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            red = qRed(line[x]);
            green = qGreen(line[x]);
            blue = qBlue(line[x]);
            brightness = red * .30 + green * .60 + blue * 0.10;
            if (lightsDelta != 0.0 && brightness > lightsTreshold) {
                deltaBrightness =  lightsDelta * (brightness - lightsTreshold) / (255.0 - lightsTreshold);
                red = red + deltaBrightness;
                green = green + deltaBrightness;
                blue = blue + deltaBrightness;
            } else if (darksDelta != 0.0 && brightness < darksTreshold) {
                deltaBrightness =  -darksDelta * (brightness - darksTreshold) / darksTreshold;
                red = red + deltaBrightness;
                green = green + deltaBrightness;
                blue = blue + deltaBrightness;
            }
            newImage.setPixel(x,y, qRgb(FilterValues::toValidValue(red), FilterValues::toValidValue(green), FilterValues::toValidValue(blue)));
        }
    }
    return newImage;
}

