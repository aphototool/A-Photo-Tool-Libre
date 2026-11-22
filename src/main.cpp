#include "aphototoollibre.h"
#include <QApplication>
#include <QClipboard>
#include <QLocale>
#include <QTranslator>

void darkMode(QApplication&);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("A Photo Tool (Libre)");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "aphototoollibre_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    APhotoToolLibre appWindow;
    appWindow.show();
    int ret = app.exec();

    // Clear clipboard before ~QApplication destroys QGuiApplication
    // so XCB clipboard handoff doesn't try to copy a QPixmap without an active GUI instance
    if (QClipboard *clipboard = QGuiApplication::clipboard()) {
        clipboard->clear();
    }

    return ret;
}
