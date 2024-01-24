/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2024 Jari Ahola
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

#include "imagecreator.h"

ImageCreator::ImageCreator()
{

}

void ImageCreator::createPreviewImage(const QImage &tempImage, Values &values) {
    values.imageOriginal = tempImage;
    if (values.imageOriginal.height() > PREVIEW_IMAGE_SIZE_LIMIT
            || values.imageOriginal.width() > PREVIEW_IMAGE_SIZE_LIMIT) {
        if (values.imageOriginal.height() > values.imageOriginal.width()) {
            values.imageOriginalScaled = values.imageOriginal
                    .scaledToHeight(PREVIEW_IMAGE_SIZE_LIMIT, Qt::SmoothTransformation);
        } else {
            values.imageOriginalScaled = values.imageOriginal
                    .scaledToWidth(PREVIEW_IMAGE_SIZE_LIMIT, Qt::SmoothTransformation);
        }
    } else {
        values.imageOriginalScaled = values.imageOriginal.copy();
    }
    values.image = values.imageOriginalScaled.copy();
}

void ImageCreator::createFullResolutionInBackground(WorkValues *workValues, Values *values) {
    // Handle locking in caller (workValues.filterMutex.lock())
    bool needsToDoWork = values->filteredTime > workValues->lastWorkTimestamp;
    if (needsToDoWork && !workValues->backgroundWorking && values->filteredTime < BackgroundControl::backgroundWorkStopped) {
        FilterValues* tempFilterValues = values->filterValues.copy();
        QFuture<QImage> future = QtConcurrent::run(&ImageCreator::backgroundApplyFilter, values->imageOriginal, tempFilterValues);
        workValues->filterWatcher.setFuture(future);
        workValues->lastWorkTimestamp = TimeUtil::getTimestamp();
        workValues->backgroundWorking = true;
    }
}

QImage ImageCreator::backgroundApplyFilter(QImage fullOriginal, FilterValues* filterValues) {
    QThread::currentThread()->setPriority(QThread::NormalPriority);
    QImage result = Filters::applyFilters(fullOriginal, *filterValues);
    return result;
}

bool ImageCreator::backgroundFilterReady(WorkValues *workValues, Values *values) {
    // Handle locking in caller (workValues.filterMutex.lock())
    workValues->backgroundWorking = false;
    if (workValues->lastWorkTimestamp >= values->filteredTime) {
        QImage tempImage = workValues->filterWatcher.future().result();
        values->image = tempImage;
        return true;
    }
    return false;
}
