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

#ifndef CROPAREAVALIDATOR_H
#define CROPAREAVALIDATOR_H

#include <qlabel.h>
#include <qradiobutton.h>
#include "../values/imagevalues.h"

enum class CropCorner {None, TopLeft, TopRight, BottomLeft, BottomRight, Top, Bottom, Left, Right, InsideMove};
enum class CropFormat {_Free, _1x1, _4x3, _3x2, _16x9, _Locked};

class CropAreaValidator
{
public:
    CropAreaValidator(QLabel *imageLabel, QRadioButton *cropLocked, CropValues *newCropValues);
    void validateCropArea(CropFormat cropFormat, CropCorner cropCorner);
    void calculateCropFormatMultiplier();

private:
    void calculateHeightCropDragging(CropFormat cropFormat);
    void calculateWidthAndHeightCropDragging(CropFormat cropFormat);
    void calculateLowerCropDragging(CropFormat cropFormat);
    double getCropFormatMultiplier(CropFormat cropFormat);
    void refreshImagePixmapSize();

    QLabel *imageLabel = nullptr;
    QRadioButton *cropLocked;
    CropValues *newCropValues = nullptr;
    double cropFormatMultiplier = 0.0;
    double imagePixmapHeight = 0.0;
    double imagePixmapWidth = 0.0;
};

#endif // CROPAREAVALIDATOR_H
