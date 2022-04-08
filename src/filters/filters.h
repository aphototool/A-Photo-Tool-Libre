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

#ifndef FILTERS_H
#define FILTERS_H

#include <QImage>
#include <QPainter>
#include "filtervalues.h"
#include "bwfilter.h"
#include "brightnessfilter.h"
#include "contrastfilter.h"
#include "lightdarkfilter.h"
#include "saturationfilter.h"
#include "huefilter.h"
#include "colortemperaturefilter.h"
#include "rotateanyanglefilter.h"
#include "cropfilter.h"
#include "resizefilter.h"

class Filters
{
public:
    Filters();
    static QImage applyFilters(const QImage &imageToFilter, const FilterValues &filterValues);
    static QImage applySizingFilters(const QImage &imageToFilter, const FilterValues &filterValues);
};

#endif // FILTERS_H
