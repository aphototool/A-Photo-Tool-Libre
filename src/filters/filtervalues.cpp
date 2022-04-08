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

#include "filtervalues.h"

FilterValues::FilterValues()
{

}

FilterValues* FilterValues::copy()
{
    FilterValues* c = new FilterValues();

    c->brightness = brightness;
    c->contrast = contrast;
    c->lights = lights;
    c->darks = darks;
    c->saturation = saturation;
    c->hue = hue;
    c->colorTemperature = colorTemperature;

    c->enableBW = enableBW;
    c->redDelta = redDelta;
    c->greenDelta = greenDelta;
    c->blueDelta = blueDelta;

    c->rotateAngle = rotateAngle;
    c->straightenAngle = straightenAngle;

    c->resizeType = resizeType;
    c->resizeValue = resizeValue;
    c->resizePercentageValue = resizePercentageValue;

    c->cropValues = cropValues == nullptr ? nullptr : cropValues->copy();

    c->imageChanged = imageChanged;

    return c;
}


int FilterValues::toValidValue(float value)
{
    if (value > 255.0) return 255;
    if (value < 0.0) return 0;
    return int(value);
}
