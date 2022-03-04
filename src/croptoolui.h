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

#ifndef CROPTOOLUI_H
#define CROPTOOLUI_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QTimer>
#include "values.h"
#include "showimageevent.h"
#include "filters/filters.h"
#include "utils/graphics.h"

namespace Ui { class MainWindow; }

enum class CropCorner {None, TopLeft, TopRight, BottomLeft, BottomRight, Top, Bottom, Left, Right, InsideMove};
enum class CropFormat {_Free, _1x1, _4x3, _3x2, _16x9, _Locked};

class CropToolUi : public QWidget
{
public:
    CropToolUi(QMainWindow *mainWin, Ui::MainWindow *ui = nullptr, Values *values = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void onCropCancelButtonClicked();
    void onCropOkButtonClicked();
    void onUseCropCheckBoxClicked(int state);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void useCropFormat();
    void onCropButtonClicked();
    void onStraightenSliderValueChanged(int value);
    void rotatePreview(int angle);
    void showCropTool();
    void hideCropTool();
    void validateCropArea();
    double getCropFormatMultiplier();
    void calculateCropFormatMultiplier();
    void calculateHeightCropDragging();
    void calculateWidthAndHeightCropDragging();
    void calculateUpperCropDragging();
    void calculateLowerCropDragging();

    Ui::MainWindow *ui = nullptr;
    QMainWindow *mainWin = nullptr;
    Values *values = nullptr;
    QImage imageToRotate;
    int angle = 0;

    CropFormat cropFormat = CropFormat::_Free;
    double cropFormatMultiplier = 1.0;
    CropValues *newCropValues = nullptr;
    CropCorner cropCorner = CropCorner::None;
    const int clickTolerance = 10;
    bool needsPostShowImageEvent = false;

    QPoint mousePresPos = QPoint();
    int imageXStart = 0;
    int imageXEnd = INT_MAX;
    int imageYStart = 0;
    int imageYEnd = INT_MAX;

    int cropXStart = 0;
    int cropXEnd = INT_MAX;
    int cropYStart = 0;
    int cropYEnd = INT_MAX;
};

#endif // CROPTOOLUI_H
