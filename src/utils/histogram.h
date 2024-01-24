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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "../utils/graphics.h"
#include "../utils/timeutil.h"
#include "../utils/backgroundcontrol.h"
#include "../filters/bwfilter.h"
#include "../values/imagevalues.h"
#include "../values/workvalues.h"

namespace Ui { class MainWindow; }

class Histogram
{
public:
    Histogram();
    static  void createHistogramInBackground(WorkValues *workValues, Values *values);
    static void backgroundHistogramReady(WorkValues *workValues, Values *values, Ui::MainWindow *ui);

private:
    static int getMaxValue(unsigned long values[]);
    static unsigned long *evaluateImage(QImage tempImage);
    static QImage drawHistogram(const QImage &image);
    static QImage backgroundApplyFilter(QImage image);
};

#endif // HISTOGRAM_H
