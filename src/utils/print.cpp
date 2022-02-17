#include "print.h"

Print::Print()
{

}

void Print::print(const QImage &image)
{
    if (image.width() <= 0) return;

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrintDialog dialog(&printer, nullptr);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QPixmap pixmap = QPixmap::fromImage(image);   // imageLabel->pixmap(Qt::ReturnByValue);
        QRect rect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }
#endif
}
