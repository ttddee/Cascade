/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QFile>
#include <QDir>
#include <QSplashScreen>
#include <QDirIterator>

#include "log.h"

#include <OpenImageIO/imagebuf.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

int main(int argc, char *argv[])
{
    // Enable scaling for high DPI screens
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    // Load font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/opensans/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/opensans/OpenSans-Bold.ttf");
    if (fontId != -1)
        a.setFont(QFont("Open Sans"));
    else
        CS_LOG_WARNING("Problem loading font.");

    // Load style sheet
    QFile f(":/style/stylesheet.qss");
    f.open(QFile::ReadOnly);
    QString style = QLatin1String(f.readAll());
    a.setStyleSheet(style);

    // Splash
    QPixmap splashImage(":/design/cascade-splash.png");
    QSplashScreen splash(splashImage);
    splash.setWindowFlag(Qt::WindowStaysOnTopHint);
    splash.show();
    splash.showMessage("Starting up...", Qt::AlignCenter, Qt::white);

    QString title = QString("Cascade Image Editor - v%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);

    Cascade::Log::Init();

    CS_LOG_INFO(title);

    // Copy the OCIO config from the resources to disk.
    // We do this so that they end up in the right place when
    // running from an AppImage.
    if (!QDir("ocio").exists())
    {
        QDir().mkdir("ocio");
        QDir().mkdir("ocio/luts");

        QFile::copy(":/ocio/config.ocio", "ocio/config.ocio");
        QFile::copy(":/ocio/luts/alexalogc.spi1d", "ocio/luts/alexalogc.spi1d");
        QFile::copy(":/ocio/luts/cineon.spi1d", "ocio/luts/cineon.spi1d");
        QFile::copy(":/ocio/luts/panalog.spi1d", "ocio/luts/panalog.spi1d");
        QFile::copy(":/ocio/luts/ploglin.spi1d", "ocio/luts/ploglin.spi1d");
        QFile::copy(":/ocio/luts/rec709.spi1d", "ocio/luts/rec709.spi1d");
        QFile::copy(":/ocio/luts/redlog.spi1d", "ocio/luts/redlog.spi1d");
        QFile::copy(":/ocio/luts/slog.spi1d", "ocio/luts/slog.spi1d");
        QFile::copy(":/ocio/luts/srgb.spi1d", "ocio/luts/srgb.spi1d");
        QFile::copy(":/ocio/luts/srgbf.spi1d", "ocio/luts/srgbf.spi1d");
        QFile::copy(":/ocio/luts/viperlog.spi1d", "ocio/luts/viperlog.spi1d");
    }
    // Same for the ISF shaders
    QDir dstDir("isf");
    if (!dstDir.exists())
    {
        QDir().mkdir("isf");
        QDir srcDir(":/shaders/isf");
        QDirIterator it(srcDir, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QFile f(it.next());
            auto split = f.fileName().split("/");
            QString dstName = "isf/" + split.last();
            f.copy(dstName);
        }
    }

    // Create window
    Cascade::MainWindow w;
    w.setWindowState(Qt::WindowMaximized);

    w.setWindowTitle(title);
    w.show();

    splash.finish(&w);

    return a.exec();
}
