/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2025 Jari Ahola
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

#ifndef IMAGELABELPAINTER_H
#define IMAGELABELPAINTER_H

#include <QLabel>
#include <QPainter>
#include <QtDebug>
#include "filters/cropvalues.h"
#include "utils/qpixmapversionedutils.h"

enum class GridFormat {Grid3x3, Golden};

class ImageLabelPainter
{
public:
    ImageLabelPainter();
    ~ImageLabelPainter();
    void paintOnLabel(QLabel *label, CropValues *cropValues);
private:
    void paintGrid(QPainter *painter, int sectors, int width, int height);
    void paintGrid(QPainter *painter, GridFormat sectors, int x1, int y1, int x2, int y2);
    void paintCropHandles(QPainter *painter, int x1, int y1, int x2, int y2);
};

#endif // IMAGELABELPAINTER_H
