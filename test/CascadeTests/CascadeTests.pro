include(gtest_dependency.pri)

QT += widgets core testlib

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
#CONFIG -= qt

CONFIG += c++17

HEADERS += \
        testheader.h \
        tst_slider.h \
        ../../src/ui/slider.h

SOURCES += \
        main.cpp \
        ../../src/ui/slider.cpp
