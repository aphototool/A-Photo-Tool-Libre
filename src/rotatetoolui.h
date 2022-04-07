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

#ifndef ROTATETOOLUI_H
#define ROTATETOOLUI_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include "values/imagevalues.h"
#include "values/workvalues.h"
#include "showimageevent.h"
#include "utils/graphics.h"
#include "utils/backgroundcontrol.h"

namespace Ui { class MainWindow; }

class RotateToolUi : public QWidget
{
public:
    RotateToolUi(QMainWindow *mainWin, Ui::MainWindow *ui, Values *values, WorkValues *workValues);
    void resizeEvent(QResizeEvent *event);
    void onRotateButtonClicked();
    void onRotateLeftButtonClicked();
    void onRotateRightButtonClicked();
    void rotatePreview(int angle);
    void onRotateCancelButtonClicked();
    void onRotateOkButtonClicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void rotateCrop(int angle);
    void showRotateTool();
    Ui::MainWindow *ui = nullptr;
    QMainWindow *mainWin = nullptr;
    Values *values = nullptr;
    WorkValues *workValues = nullptr;
    QImage imageToRotate;
    int angle = 0;
    CropValues *newCropValues = nullptr;
    int newFilteredImageWidth = 0;
    int newFilteredImageHeight = 0;
    ResizeCalcType newResizeCalcType = ResizeCalcType::None;
};

#endif // ROTATETOOLUI_H
