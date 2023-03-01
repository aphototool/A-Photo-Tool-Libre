/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2023 Jari Ahola
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

#include "histogram.h"
#include "ui_aphototoollibre.h"

Histogram::Histogram()
{
}

void Histogram::createHistogramInBackground(WorkValues *workValues, Values *values) {
    // Handle locking in caller (workValues.filterMutex.lock())
    bool needsToDoWork = values->filteredTime > workValues->lastWorkTimestamp;
    if (needsToDoWork && !workValues->backgroundWorking && values->filteredTime < BackgroundControl::backgroundWorkStopped) {
        QFuture<QImage> future = QtConcurrent::run(&Histogram::backgroundApplyFilter, values->image.copy());
        workValues->filterWatcher.setFuture(future);
        workValues->lastWorkTimestamp = TimeUtil::getTimestamp();
        workValues->backgroundWorking = true;
    }
}

QImage Histogram::backgroundApplyFilter(QImage image) {
    QThread::currentThread()->setPriority(QThread::NormalPriority);
    QImage result = Histogram::drawHistogram(image);
    return result;
}

void Histogram::backgroundHistogramReady(WorkValues *workValues, Values *values,  Ui::MainWindow *ui) {
    // Handle locking in caller (workValues.filterMutex.lock())
    workValues->backgroundWorking = false;
    if (workValues->lastWorkTimestamp >= values->filteredTime) {
        QImage tempImage = workValues->filterWatcher.future().result();
        Graphics::fitImage(tempImage, *ui->histogramLabel);
    }
}


QImage Histogram::drawHistogram(const QImage &image)
{
    QImage histImage = QImage(256, 256 * 3 / 4, QImage::Format_ARGB32);
    histImage.fill(QColor(150, 150, 150));

    QImage tempImage = BWFilter().toBAndW(image, 30.0, 60.0, 10.0);
    unsigned long * values = evaluateImage(tempImage);
    unsigned long maxValue =  Histogram::getMaxValue(values);

    double hy = histImage.height();
    double yMargin = hy * 1 / 5;
    double hyVirtual = hy - yMargin;
    QPainter painter(&histImage);
    painter.setBrush(Qt::NoBrush);
    QColor color(255, 255, 255, 255);
    QColor colorRed(255, 0, 0, 255);
    QPen pen(color);
    double penWidht = histImage.width() / 256.0;
    pen.setWidth(penWidht);
    double yValue = 0.0;
    for (int x = 0; x < 256; x++)
    {
        yValue = hyVirtual - double(values[x]) / double(maxValue) * hyVirtual;
        if (yValue < -yMargin) {
            yValue = 0.0;
            pen.setColor(colorRed);
            painter.setPen(pen);
            qDebug("Punainen");
        } else {
            pen.setColor(color);
            painter.setPen(pen);
            yValue += yMargin;
        }
        qDebug("%1f", yValue);
        painter.drawLine(x * penWidht, hy, x * penWidht, yValue);
    }
    delete[] values;
    return histImage;
}

unsigned long * Histogram::evaluateImage(QImage tempImage) {
    const unsigned long max_ulong = std::numeric_limits<unsigned long>::max();
    unsigned long *values = new unsigned long[256];
    for (int i = 0; i < 256; i++) {
        values[i] = 0;
    }
    for(int y = 0; y<tempImage.height(); y++){
        QRgb * line = (QRgb *)tempImage.scanLine(y);
        for(int x = 0; x<tempImage.width(); x++){
            int value = qRed(line[x]);
            if (values[value] < max_ulong) {
                values[value]++;
            }
        }
    }
    return values;
}

int Histogram::getMaxValue(unsigned long values[]) {
    unsigned long maxValue = 1.0;
    int firstIndex = 0;
    for (int i = 0; i < 256; i++) {
        if (values[i] > 0) {
            firstIndex = i;
            break;
        }
    }
    int lastIndex = 255;
    for (int i = 255; i >= 0; i--) {
        if (values[i] > 0) {
            lastIndex = i;
            break;
        }
    }

    for (int i = firstIndex + 1; i < lastIndex; i++) {
        if (values[i] > maxValue) {
            maxValue = values[i];
        }
    }
    return maxValue;
}
