#include "stylemode.h"
#include "./ui_aphototoollibre.h"

StyleMode::StyleMode()
{

}

void StyleMode::darkMode(QApplication *app, Ui::MainWindow *ui) {
    app->setStyle("Fusion");
    QPalette dark_palette = QPalette();
    dark_palette.setColor(QPalette::Window, QColor(55, 55, 55));
    dark_palette.setColor(QPalette::WindowText, Qt::white);
    dark_palette.setColor(QPalette::Disabled, QPalette::WindowText, Qt::darkGray);
    dark_palette.setColor(QPalette::Base, QColor(35, 35, 35));
    dark_palette.setColor(QPalette::AlternateBase, QColor(55, 55, 55));
    dark_palette.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
    dark_palette.setColor(QPalette::ToolTipText, Qt::white);
    dark_palette.setColor(QPalette::Text, Qt::white);
    dark_palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    dark_palette.setColor(QPalette::Button, QColor(55, 55, 55));
    // dark_palette.setColor(QPalette::Disabled, QPalette::Light, QColor(55, 55, 55));
    dark_palette.setColor(QPalette::ButtonText, Qt::white);
    dark_palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    dark_palette.setColor(QPalette::BrightText, Qt::red);
    dark_palette.setColor(QPalette::Link, QColor(40, 100, 250));
    dark_palette.setColor(QPalette::Highlight, QColor(100, 100, 100));
    dark_palette.setColor(QPalette::HighlightedText, QColor(35, 35, 35));
    dark_palette.setColor(QPalette::Active, QPalette::Button, QColor(55, 55, 55));
    app->setPalette(dark_palette);

    ui->redSlider->setStyleSheet(getStyleSheetForColorSlider("red", "#232323"));
    ui->greenSlider->setStyleSheet(getStyleSheetForColorSlider("green", "#232323"));
    ui->blueSlider->setStyleSheet(getStyleSheetForColorSlider("blue", "#232323"));
    ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : #232323; }");
}

void StyleMode::lightMode(QApplication *app, Ui::MainWindow *ui) {
    app->setStyle("");
    app->setPalette(QPalette());
    ui->redSlider->setStyleSheet(getStyleSheetForColorSlider("red", "lightGray"));
    ui->greenSlider->setStyleSheet(getStyleSheetForColorSlider("green", "lightGray"));
    ui->blueSlider->setStyleSheet(getStyleSheetForColorSlider("blue", "lightGray"));
    ui->scrollAreaWidgetContents->setStyleSheet("QWidget { background-color : lightGray; }");
}

QString StyleMode::getStyleSheetForColorSlider(const QString fgColor, const QString bgColor) {
    return QString("QSlider::groove:vertical { background: black; position: absolute; left: 6px; right: 6px; width: 4px } "
                   "QSlider::handle:vertical { height: 14px; background: %1; margin: 0 -6px; border: 1px solid gray; border-radius: 8px;} "
                   "QSlider::add-page:vertical { background: %1; } "
                   "QSlider::sub-page:vertical { background: %2; }").arg(fgColor, bgColor);
}
