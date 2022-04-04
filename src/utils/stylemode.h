#ifndef STYLEMODE_H
#define STYLEMODE_H

#include <QApplication>
#include <QPalette>

namespace Ui { class MainWindow; }

class StyleMode
{
public:
    StyleMode();
    static void darkMode(QApplication *app, Ui::MainWindow *ui);
    static void lightMode(QApplication *app, Ui::MainWindow *ui);
private:
    Ui::MainWindow *ui = nullptr;
    static QString getStyleSheetForColorSlider(const QString fgColor, const QString bgColor);
};

#endif // STYLEMODE_H
