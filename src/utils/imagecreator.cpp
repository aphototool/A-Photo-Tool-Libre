#include "imagecreator.h"
#include "ui_aphototoollibre.h"

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
    bool needsToDoWork = values->filteredTime > workValues->lastFullResTimestamp;
    if (needsToDoWork && !workValues->backgroundWorking && values->filteredTime < BackgroundControl::backgroundWorkStopped) {
        FilterValues* tempFilterValues = values->filterValues.copy();
        QFuture<QImage> future = QtConcurrent::run(&ImageCreator::backgroundApplyFilter, values->imageOriginal, tempFilterValues);
        workValues->filterWatcher.setFuture(future);
        workValues->lastFullResTimestamp = TimeUtil::getTimestamp();
        workValues->backgroundWorking = true;
    }
}

QImage ImageCreator::backgroundApplyFilter(QImage fullOriginal, FilterValues* filterValues) {
    QThread::currentThread()->setPriority(QThread::NormalPriority);
    QImage result = Filters::applyFilters(fullOriginal, *filterValues);
    return result;
}

void ImageCreator::backgroundFilterReady(WorkValues *workValues, Values *values,  Ui::MainWindow *ui) {
    // Handle locking in caller (workValues.filterMutex.lock())
    workValues->backgroundWorking = false;
    if (workValues->lastFullResTimestamp >= values->filteredTime) {
        QImage tempImage = workValues->filterWatcher.future().result();
        values->image = tempImage;
        ui->scrollArea->setWidgetResizable(true);
        Graphics::fitImage(values->image, *ui->imageLabel);
        ui->previewLabel->setText(tr("View: Full Res"));
    }
}
