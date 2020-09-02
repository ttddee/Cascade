#include "mainwindow.h"

#include <iostream>

#include <QApplication>
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Bold.ttf");
    if (fontId != -1)
    {
        a.setFont(QFont("Source Code Pro"));
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
