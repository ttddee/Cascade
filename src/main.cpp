/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#include <iostream>

#include <QApplication>
#include <QFontDatabase>
#include <QFile>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

    // Load font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/opensans/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/opensans/OpenSans-Bold.ttf");
    if (fontId != -1)
    {
        a.setFont(QFont("Open Sans"));
    }
    else
    {
        std::cout << "Problem loading font." << std::endl;
    }

    // Load style sheet
    QFile f(":/style/stylesheet.qss");
    f.open(QFile::ReadOnly);
    QString style = QLatin1String(f.readAll());
    a.setStyleSheet(style);

    // Create window
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    auto title = QString("Cascade Image Editor - v%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
    w.setWindowTitle(title);
    w.show();
    return a.exec();
}
