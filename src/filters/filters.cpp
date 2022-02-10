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

#include "filters.h"

Filters::Filters()
{

}

QImage Filters::applyFilters(const QImage &imageToFilter, const FilterValues &filterValues)
{
    QImage tempImage = imageToFilter;
    if (filterValues.rotateAngle != 0 || filterValues.straightenAngle != 0) {
        tempImage = RotateAnyAngleFilter().rotateImage(tempImage, filterValues.rotateAngle, filterValues.straightenAngle);
    }
    if (filterValues.cropValues != nullptr && filterValues.cropValues->useCrop) {
        tempImage = CropFilter().cropImage(tempImage, *filterValues.cropValues);
    }
    if (filterValues.resizeValue > 0.0 && filterValues.resizeType != ResizeCalcType::None) {
        tempImage = ResizeFilter().resizeImage(tempImage,
                                               filterValues.resizeValue,
                                               filterValues.resizePercentageValue,
                                               filterValues.resizeType);
    }
    if (filterValues.lightness != 0.0) {
        tempImage = LightnessFilter().adjustLightness(tempImage, filterValues.lightness);
    }
    if (filterValues.contrast != 0.0) {
        tempImage = ContrastFilter().adjustContrast(tempImage, filterValues.contrast);
    }
    if (filterValues.saturation != 0.0) {
        tempImage = SaturationFilter().adjustSaturation(tempImage, filterValues.saturation);
    }
    if (filterValues.hue != 0.0) {
        tempImage = HueFilter().adjustHue(tempImage, filterValues.hue);
    }
    if (filterValues.colorTemperature != 0.0) {
        tempImage = ColorTemperatureFilter().changeColorTemperature(tempImage, filterValues.colorTemperature);
    }
    if (filterValues.enableBW && (filterValues.redDelta > 0.0 || filterValues.greenDelta > 0.0 || filterValues.blueDelta > 0.0)) {
        tempImage = BWFilter().toBAndW(tempImage, filterValues.redDelta, filterValues.greenDelta, filterValues.blueDelta);
    }
    return tempImage;
}

