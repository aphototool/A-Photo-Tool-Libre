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
