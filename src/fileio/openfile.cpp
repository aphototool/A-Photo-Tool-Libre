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

#include "openfile.h"

OpenFile::OpenFile(QWidget *parent) : QWidget(parent)
{

}

QImage OpenFile::open()
{
    fileName = "";
    height = 0;
    width = 0;
    QFileDialog dialog(this, tr("Open File"));
    BaseFile::initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
    dialog.setOption(QFileDialog::DontUseNativeDialog, DONT_USE_NATIVE_DIALOG);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
    return image;
}

QImage OpenFile::open(const QString &fileName)
{
    height = 0;
    width = 0;
    loadFile(fileName);
    return image;
}

bool OpenFile::loadFile(const QString &fileName)
{
    this->fileName = fileName;
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(parentWidget(), QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    this->height = newImage.height();
    this->width = newImage.width();

    setImage(newImage);

    setWindowFilePath(fileName);

    return true;
}

void OpenFile::setImage(const QImage &newImage)
{
    image = newImage;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
#endif
}

QString OpenFile::getFileName() {
    return fileName;
}
