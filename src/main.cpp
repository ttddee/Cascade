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
