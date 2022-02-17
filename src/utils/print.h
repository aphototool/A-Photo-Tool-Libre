#ifndef PRINT_H
#define PRINT_H

#if defined(QT_PRINTSUPPORT_LIB)
#  include <QtPrintSupport/qtprintsupportglobal.h>

#  if QT_CONFIG(printer)
#    include <QPrinter>
#  endif
#  if QT_CONFIG(printdialog)
#    include <QPrintDialog>
#    include <QPainter>
#  endif
#endif

#include <QImage>

class Print // : public QWidget
{
public:
    Print();
    void print(const QImage &image);


private:

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printer;
#endif

};

#endif // PRINT_H
