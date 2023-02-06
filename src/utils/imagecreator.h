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

#ifndef IMAGECREATOR_H
#define IMAGECREATOR_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <QString>
#include "../filters/filters.h"
#include "../values/imagevalues.h"
#include "../values/workvalues.h"
#include "../utils/graphics.h"
#include "../utils/backgroundcontrol.h"
#include "../utils/timeutil.h"
#include "../constants.h"

namespace Ui { class MainWindow; }

class ImageCreator {

public:
    ImageCreator();
    static void createPreviewImage(const QImage &tempImage, Values &values);
    static QImage backgroundApplyFilter(QImage fullOriginal, FilterValues *filterValues);
    static void backgroundFilterReady(WorkValues *workValues, Values *values, Ui::MainWindow *ui);
    static void createFullResolutionInBackground(WorkValues *workValues, Values *values);
};

#endif // IMAGECREATOR_H
