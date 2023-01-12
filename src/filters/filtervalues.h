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

#ifndef FILTERVALUES_H
#define FILTERVALUES_H

#include "filters/cropvalues.h"

enum class ResizeCalcType { None, Height, Width, Percentage };

class FilterValues
{
public:
    FilterValues();
    FilterValues* copy();

    float brightness = 0.0;
    float contrast = 0.0;
    float lights = 0.0;
    float darks = 0.0;
    float saturation = 0.0;
    float hue = 0.0;
    float colorTemperature = 0.0;
    float tintValue = 0.0;

    bool enableBW = false;
    float redDelta = 30.0;
    float greenDelta = 60.0;
    float blueDelta = 10.0;

    int rotateAngle = 0;
    int straightenAngle = 0;

    ResizeCalcType resizeType = ResizeCalcType::None;
    double resizeValue = 0.0;
    double resizePercentageValue = 100.0;

    CropValues *cropValues = nullptr;

    bool imageChanged = false;

    static int toValidValue(float value);
};

#endif // FILTERVALUES_H
