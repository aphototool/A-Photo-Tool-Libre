#ifndef CROPAREAVALIDATOR_H
#define CROPAREAVALIDATOR_H

#include <qlabel.h>
#include <qradiobutton.h>
#include "../values.h"

enum class CropCorner {None, TopLeft, TopRight, BottomLeft, BottomRight, Top, Bottom, Left, Right, InsideMove};
enum class CropFormat {_Free, _1x1, _4x3, _3x2, _16x9, _Locked};

class CropAreaValidator
{
public:
    CropAreaValidator(QLabel *imageLabel, QRadioButton *cropLocked, CropValues *newCropValues);
    void validateCropArea(CropFormat cropFormat, CropCorner cropCorner);
    void calculateCropFormatMultiplier();

private:
    void calculateHeightCropDragging(CropFormat cropFormat);
    void calculateWidthAndHeightCropDragging(CropFormat cropFormat);
    void calculateLowerCropDragging(CropFormat cropFormat);
    double getCropFormatMultiplier(CropFormat cropFormat);

    QLabel *imageLabel = nullptr;
    QRadioButton *cropLocked;
    CropValues *newCropValues = nullptr;
    double cropFormatMultiplier = 0.0;
};

#endif // CROPAREAVALIDATOR_H
