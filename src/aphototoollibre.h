/*
 * A Photo Tool (Libre)
 *
 * Copyright © 2021-2022 Jari Ahola
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

#ifndef APHOTOTOOLLIBRE_H
#define APHOTOTOOLLIBRE_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QtDebug>
#include <QSettings>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QClipboard>
#include <QMimeData>
#include <QMenu>
#include "constants.h"
#include "fileio/openfile.h"
#include "fileio/savefile.h"
#include "filters/filters.h"
#include "utils/print.h"
#include "values/imagevalues.h"
#include "rotatetoolui.h"
#include "croptoolui.h"
#include "resizetoolui.h"
#include "showimageevent.h"
#include "utils/stylemode.h"
#include "utils/settingsutils.h"
#include "utils/imagecreator.h"
#include "optionsdialog.h"
#include "values/usersettingvalues.h"
#include "utils/timeutil.h"
#include "values/workvalues.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QSlider;
class QPushButton;
class QCheckBox;
class QCustomEvent;
QT_END_NAMESPACE

class APhotoToolLibre : public QMainWindow
{
    Q_OBJECT

public:
    APhotoToolLibre(QWidget *parent = nullptr);
    ~APhotoToolLibre();
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

    bool event(QEvent *event);

    void onDropImage(QImage droppedImage);
    void loadImage(const QString filename);

    UserSettingValues* getAppSettings();

    Ui::MainWindow *getUi() const;

private slots:
    void onClipboardChange();

private:
    void onLoadImageButtonClicked();
    void onSaveButtonClicked();
    void onPrintClicked();
    void onCloseWindowClicked();

    void onResetExposureClicked();
    void onResetColorsClicked();

    void onEnableBWCheckBoxStateChanged(int arg1);
    void onNaturalButtonClicked();
    void onPanchromaticButtonClicked();

    void onBrightnessSliderValueChanged(int value);
    void onContrastSliderValueChanged(int value);
    void onLightsSliderValueChanged(int value);
    void onDarksSliderValueChanged(int value);
    void onSaturationSliderValueChanged(int value);
    void onHueSliderValueChanged(int value);
    void onColorTemperatureSliderValueChanged(int value);
    void onRedSliderValueChanged(int value);
    void onGreenSliderValueChanged(int value);
    void onBlueSliderValueChanged(int value);

    void onClippoardCopy();
    void onClippoardPaste();

    void checkForUpdates();

    void loadImage(QString fileName, QImage tempImage);
    void saveFullResImage();
    bool isLoadNewPhtoOk();
    void resetValues();
    void showImage(const QImage &imageToShow);
    void showPreviewImage();
    void showFullResolutionImage();
    void setExposureSliders();
    void setColorSliders();
    void setBWSliders();
    void setBWValueLabels();

    void applySizingFiltersToGetNewSize();
    void showFileInfo();

    QString getStyleSheetForColorSlider(const QString color);

    void about();

    void writeSettings();
    void readSettings();
    void showSettings();

    Ui::MainWindow *ui;
    UserSettingValues appSettings;
    RotateToolUi *rotateToolUi;
    CropToolUi *cropToolUi;
    ResizeToolUi *resizeToolUi;
    QSlider *redBWSlider;
    Values values;
    QString argFileName = nullptr;

    void createFullResolutionInBackground();
    void backgroundFilterReady();
    WorkValues workValues;
};
#endif // APHOTOTOOLLIBRE_H
