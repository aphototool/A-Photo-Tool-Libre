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

#include "resizefilter.h"

ResizeFilter::ResizeFilter()
{

}

QImage ResizeFilter::resizeImage(const QImage &image, double resizeValue, double resizePercentageValue, ResizeCalcType resizeType)
{
    QImage newImage = image.copy();
    switch (resizeType) {
    case ResizeCalcType::Width:
        newImage = newImage.scaledToWidth(resizeValue, Qt::SmoothTransformation);
        break;
    case ResizeCalcType::Height:
        newImage = newImage.scaledToHeight(resizeValue, Qt::SmoothTransformation);
        break;
    case ResizeCalcType::Percentage:
    {
        double scale = resizePercentageValue / 100.0;
        if (newImage.width() > newImage.height()) {
            newImage = newImage.scaledToWidth((int)(newImage.width() * scale),
                                                Qt::SmoothTransformation);
        } else {
            newImage = newImage.scaledToHeight((int)(newImage.height() * scale),
                                                Qt::SmoothTransformation);
        }
        break;
    }
    default:
        break;
    }
    return newImage;
}
