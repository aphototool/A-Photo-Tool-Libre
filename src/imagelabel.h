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

#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include "aphototoollibre.h"
#include "imagelabelpainter.h"
#include "filters/cropvalues.h"

class ImageLabel : public QLabel
{
public:
    ImageLabel(QWidget* parent = nullptr);
    ImageLabelPainter* imageLabelPainter = nullptr;
    CropValues *cropValues = nullptr;

    void setLoadImageFunction(APhotoToolLibre *parent);
    void setLoadImageFunction(void (APhotoToolLibre::*)(QImage));

protected:
    void paintEvent(QPaintEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void loadImage(QString fileName, QImage image);
    APhotoToolLibre* mainWindow;
};

#endif // IMAGELABEL_H
