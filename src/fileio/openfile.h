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

#ifndef OPENFILE_H
#define OPENFILE_H

#include <QObject>
#include <QWidget>
#include <QImageReader>
#include <QMessageBox>
#include <QGuiApplication>
#include <QDir>
#include <QTranslator>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageWriter>
#include <QImage>
#include <QColorSpace>

#include "basefile.h"
#include "constants.h"

class OpenFile : public QWidget
{
    Q_OBJECT
public:
    OpenFile(QWidget *parent = nullptr);
    QImage open();
    QImage open(const QString &fileName);
    bool loadFile(const QString &);
    QString getFileName();
private:
    void setImage(const QImage &newImage);

    QImage image;
    QString fileName;
    int height = 0;
    int width = 0;
};

#endif // OPENFILE_H
