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

#include "savefile.h"

SaveFile::SaveFile(QWidget *parent) : QWidget(parent)
{

}

void SaveFile::saveAs(Values *values)
{
    QFileDialog dialog(this, tr("Save File As"));
    BaseFile::initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::DontUseNativeDialog, DONT_USE_NATIVE_DIALOG);
    dialog.selectFile(values->originaFileName);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst(), values)) {}
}

bool SaveFile::saveFile(const QString &fileName, Values *values)
{
    QImageWriter writer(fileName);

    if (writer.supportsOption(QImageIOHandler::Quality)) {
        writer.setQuality(85);
    }

    if (!writer.write(values->image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), writer.errorString()));
        return false;
    }
    values->imageModified = false;
    return true;
}
