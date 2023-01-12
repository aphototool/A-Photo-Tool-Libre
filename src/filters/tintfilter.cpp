#include "tintfilter.h"

TintFilter::TintFilter()
{

}


/*
 * This filter function mimicks tint change to green or magenta.
 */
QImage TintFilter::changeTint(const QImage &image, float tintValue)
{
    QImage newImage = image.copy();

    if (tintValue == 0.0) return newImage;

    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    for(int y = 0; y<newImage.height(); y++){
        QRgb * line = (QRgb *)image.scanLine(y);
        for(int x = 0; x<newImage.width(); x++){
            if (tintValue > 0.0) {
                red = qRed(line[x]) + tintValue;
                green = qGreen(line[x]);
                blue = qBlue(line[x]) + tintValue / 2.0;
            } else {
                red = qRed(line[x]) - tintValue / 2.0;
                green = qGreen(line[x]) - tintValue;
                blue = qBlue(line[x]);
            }
            newImage.setPixel(x,y, qRgb(FilterValues::toValidValue(red), FilterValues::toValidValue(green), FilterValues::toValidValue(blue)));
        }
    }
    return newImage;
}
