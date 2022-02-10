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

#include "saturationfilter.h"

SaturationFilter::SaturationFilter()
{

}

QImage SaturationFilter::adjustSaturation(const QImage &image, float saturation)
{
    QImage newImage = image.copy();

    QColor newColor;
    int h,s,l;

    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            newColor = QColor(line[x]).toHsl();
            h = newColor.hue();
            s = newColor.saturation()+int(saturation);
            s = qBound(0, s, 255);
            l = newColor.lightness();
            newColor.setHsl(h, s, l);
            newImage.setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }
    return newImage;
}
