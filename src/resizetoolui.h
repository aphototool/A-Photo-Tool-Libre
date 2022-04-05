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

#ifndef RESIZETOOLUI_H
#define RESIZETOOLUI_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include "values/imagevalues.h"
#include "showimageevent.h"

namespace Ui { class MainWindow; }

class ResizeToolUi : public QWidget
{
public:
    ResizeToolUi(QMainWindow *mainWin, Ui::MainWindow *ui = nullptr, Values *values = nullptr);
    void onResizeButtonClicked();
    void onResizeCancelButtonClicked();
    void onResizeOkButtonClicked();
    void onWidthChanged();
    void onHeightChanged();
    void onPercentageChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void showResizeTool();
    void setFieldValues();
    void calculateSize(ResizeCalcType type);
    Ui::MainWindow *ui = nullptr;
    QMainWindow *mainWin = nullptr;
    Values *values = nullptr;

    ResizeCalcType resizeType = ResizeCalcType::None;
    int newWidth = 0;
    int newHeight = 0;
    double newPercentage = 0.0;
    int oldWidth = 0;
    int oldHeight = 0;


    const QString lineEditErrorStyle = "border: 1px solid red";
};

#endif // RESIZETOOLUI_H
