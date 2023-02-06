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

#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget* parent) : QLabel(parent)
{
    setAcceptDrops(true);
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if (imageLabelPainter != nullptr) imageLabelPainter->paintOnLabel(this, cropValues);
}

void ImageLabel::setLoadImageFunction(APhotoToolLibre *mainWinodw) {
    this->mainWindow = mainWinodw;
}

void ImageLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasImage()) {
        event->acceptProposedAction();
    } else if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ImageLabel::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasImage()) {
        QImage image = event->mimeData()->imageData().value<QImage>();
        mainWindow->onDropImage(image);
        event->acceptProposedAction();
    } else if (event->mimeData()->hasUrls()) {
        QUrl url = event->mimeData()->urls().first();
        if (url.isLocalFile()) {
            mainWindow->loadImage(url.toLocalFile());
            event->acceptProposedAction();
        }
    }
}
