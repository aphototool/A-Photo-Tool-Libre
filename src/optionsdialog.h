#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QMainWindow>
// #include "aphototoollibre.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();

    void prepareSettingsDialog(QMainWindow *newParentClass);

private:
    void lightMode();
    void darkMode();
    void useDarkMode(bool darkMode);

    QMainWindow *parentClass;

    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
