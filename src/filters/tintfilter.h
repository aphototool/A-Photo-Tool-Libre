#ifndef TINTFILTER_H
#define TINTFILTER_H

#include <QImage>
#include "filtervalues.h"

class TintFilter
{
public:
    TintFilter();
    QImage changeTint(const QImage &image, float tintValue);
};

#endif // TINTFILTER_H
