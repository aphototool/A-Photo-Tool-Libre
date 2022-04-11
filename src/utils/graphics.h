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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include "../values/imagevalues.h"

class Graphics
{
public:
    Graphics();
    static QImage drawGrid(const QImage &imageToDrawOn, const int sectors);
    static void fitImage(const QImage &imageToFit, QLabel &imageLabel);
    static void showFileInfo(Values values, QLabel *infoLabel, QMainWindow *window);
};

#endif // GRAPHICS_H
