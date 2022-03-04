#include "cropareavalidator.h"

CropAreaValidator::CropAreaValidator(QLabel *imageLabel, QRadioButton *cropLocked, CropValues *newCropValues)
{
    this->imageLabel = imageLabel;
    this->newCropValues = newCropValues;
    this->cropLocked = cropLocked;
}

void CropAreaValidator::validateCropArea(CropFormat cropFormat, CropCorner cropCorner)
{
    if (imageLabel == nullptr || newCropValues == nullptr) return;

    double imageLabelHeight = (double)imageLabel->height();
    double imageLabelWidth = (double)imageLabel->width();
    double marginH = 40.0 / imageLabelHeight;
    double marginW = 40.0 / imageLabelWidth;

    if (newCropValues->x1 < 0.0) newCropValues->x1 = 0.0;
    if (newCropValues->x1 > 1.0 - marginW) newCropValues->x1 = 1.0 - marginW;
    if (newCropValues->y1 < 0.0) newCropValues->y1 = 0.0;
    if (newCropValues->y1 > 1.0 - marginH) newCropValues->y1 = 1.0 - marginH;
    if (newCropValues->x1 > newCropValues->x2 - marginW) newCropValues->x2 = newCropValues->x1 + marginW;
    if (newCropValues->y1 > newCropValues->y2 - marginH) newCropValues->y2 = newCropValues->y1 + marginH;
    if (newCropValues->x2 < marginW) newCropValues->x2 = marginW;
    if (newCropValues->x2 > 1.0) newCropValues->x2 = 1.0;
    if (newCropValues->y2 < marginH) newCropValues->y2 = marginH;
    if (newCropValues->y2 > 1.0) newCropValues->y2 = 1.0;
    if (cropFormat != CropFormat::_Free) {
        switch (cropCorner) {
            case CropCorner::TopLeft:
                calculateWidthAndHeightCropDragging(cropFormat);
                break;
            case CropCorner::TopRight:
                calculateWidthAndHeightCropDragging(cropFormat);
                break;
            case CropCorner::BottomLeft:
                calculateLowerCropDragging(cropFormat);
                break;
            case CropCorner::BottomRight:
                calculateLowerCropDragging(cropFormat);
                break;
            case CropCorner::Top:
                calculateHeightCropDragging(cropFormat);
                break;
            case CropCorner::Bottom:
                calculateHeightCropDragging(cropFormat);
                break;
            case CropCorner::Left:
                calculateWidthAndHeightCropDragging(cropFormat);
                break;
            case CropCorner::Right:
                calculateWidthAndHeightCropDragging(cropFormat);
                break;
            case CropCorner::InsideMove:
                break;
            default:
                calculateWidthAndHeightCropDragging(cropFormat);
                break;

        }
    }
    calculateCropFormatMultiplier();
}

void CropAreaValidator::calculateHeightCropDragging(CropFormat cropFormat) {
    double imageHeight = (double)imageLabel->pixmap().height();
    double imageWidth = (double)imageLabel->pixmap().width();
    double w;
    double h = newCropValues->y2 - newCropValues->y1;
    if (imageWidth >= imageHeight) {
        w = h * imageHeight / imageWidth * getCropFormatMultiplier(cropFormat);
        newCropValues->x1 = newCropValues->x2 - w;
        if (newCropValues->x1 < 0.0) {
            newCropValues->x1 = 0.0;
            w = newCropValues->x2 - newCropValues->x1;
            newCropValues->y2 = newCropValues->y1 + w * imageWidth / imageHeight / getCropFormatMultiplier(cropFormat);
        }
    } else {
        w = h * imageHeight / imageWidth / getCropFormatMultiplier(cropFormat);
        newCropValues->x1 = newCropValues->x2 - w;
        if (newCropValues->x1 < 0.0) {
            newCropValues->x1 = 0.0;
            w = newCropValues->x2 - newCropValues->x1;
            newCropValues->y2 = newCropValues->y1 + w * imageWidth / imageHeight * getCropFormatMultiplier(cropFormat);
        }
    }
}

void CropAreaValidator::calculateWidthAndHeightCropDragging(CropFormat cropFormat) {
    double imageHeight = (double)imageLabel->pixmap().height();
    double imageWidth = (double)imageLabel->pixmap().width();
    double w = newCropValues->x2 - newCropValues->x1;
    double h;
    if (imageWidth >= imageHeight) {
        h = w * imageWidth / imageHeight / getCropFormatMultiplier(cropFormat);
        newCropValues->y1 = newCropValues->y2 - h;
        if (newCropValues->y1 < 0.0) {
            newCropValues->y1 = 0.0;
            h = newCropValues->y2 - newCropValues->y1;
            newCropValues->x2 = newCropValues->x1 + h * imageHeight / imageWidth * getCropFormatMultiplier(cropFormat);
        }
    } else {
        h = w * imageWidth / imageHeight * getCropFormatMultiplier(cropFormat);
        newCropValues->y1 = newCropValues->y2 - h;
        if (newCropValues->y1 < 0.0) {
            newCropValues->y1 = 0.0;
            h = newCropValues->y2 - newCropValues->y1;
            newCropValues->x2 = newCropValues->x1 + h * imageHeight / imageWidth / getCropFormatMultiplier(cropFormat);
        }
    }
}

void CropAreaValidator::calculateLowerCropDragging(CropFormat cropFormat) {
    double imageHeight = (double)imageLabel->pixmap().height();
    double imageWidth = (double)imageLabel->pixmap().width();
    double w = newCropValues->x2 - newCropValues->x1;
    double h;
    if (imageWidth >= imageHeight) {
        h = w * imageWidth / imageHeight / getCropFormatMultiplier(cropFormat);
        newCropValues->y2 = newCropValues->y1 + h;
        if (newCropValues->y2 > 1.0) {
            newCropValues->y2 = 1.0;
            h = newCropValues->y2 - newCropValues->y1;
            newCropValues->x2 = newCropValues->x1 + h * imageHeight / imageWidth * getCropFormatMultiplier(cropFormat);
        }
    } else {
        h = w * imageWidth / imageHeight * getCropFormatMultiplier(cropFormat);
        newCropValues->y2 = newCropValues->y1 + h;
        if (newCropValues->y2 > 1.0) {
            newCropValues->y2 = 1.0;
            h = newCropValues->y2 - newCropValues->y1;
            newCropValues->x2 = newCropValues->x1 + h * imageWidth / imageHeight / getCropFormatMultiplier(cropFormat);
        }
    }
}


void CropAreaValidator::calculateCropFormatMultiplier() {
    double imageHeight = (double)imageLabel->pixmap().height();
    double imageWidth = (double)imageLabel->pixmap().width();
    double w = newCropValues->x2 - newCropValues->x1;
    double h = newCropValues->y2 - newCropValues->y1;
    w = w * imageWidth;
    h = h * imageHeight;
    if (imageWidth >= imageHeight) {
        cropFormatMultiplier = w / h;
    } else {
        cropFormatMultiplier = h / w;
    }
    double displayMultiplier = std::max(w / h, h / w);
    if (w >= h) {
        cropLocked->setText(QString("Locked %1 : 1").arg(displayMultiplier, 6, 'f', 3));
    } else {
        cropLocked->setText(QString("Locked 1 : %1").arg(displayMultiplier, 6, 'f', 3));
    }
}

double CropAreaValidator::getCropFormatMultiplier(CropFormat cropFormat) {
    switch (cropFormat) {
        case CropFormat::_1x1:
            return 1.0;
        case CropFormat::_4x3:
            return 4.0 / 3.0;
        case CropFormat::_3x2:
            return 3.0 / 2.0;
        case CropFormat::_16x9:
            return 16.0 / 9.0;
        case CropFormat::_Locked:
            return cropFormatMultiplier;
        default:
            return 0.0;
    }
}

