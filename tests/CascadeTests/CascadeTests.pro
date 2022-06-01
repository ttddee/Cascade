QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

CONFIG += c++17

DEFINES += GTEST_LANG_CXX11

include(../../Cascade.pro)

HEADERS += \
        testheader.h \
        tst_slider.h

SOURCES += \
        main.cpp \

linux-g++ {

}

win32-msvc* {
    INCLUDEPATH += \
        ../../vcpkg_installed/x64-windows/include \
}



