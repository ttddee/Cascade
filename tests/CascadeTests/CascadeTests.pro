QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

CONFIG += c++17

DEFINES += \
    GTEST_LANG_CXX11

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 2

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

HEADERS += \
        tst_nodebasetests.h \
        tst_testtest.h \
        ../../src/colorbutton.h \
        ../../src/colorbuttonentity.h \
        ../../src/comboboxentity.h \
        ../../src/connection.h \
        ../../src/csimage.h \
        ../../src/csslider.h \
        ../../src/fileboxentity.h \
        ../../src/mainwindow.h \
        ../../src/csmessagebox.h \
        ../../src/nodebase.h \
        ../../src/nodedefinitions.h \
        ../../src/nodegraph.h \
        ../../src/nodegraphcontextmenu.h \
        ../../src/nodeinput.h \
        ../../src/nodeoutput.h \
        ../../src/nodeproperties.h \
        ../../src/propertiesheading.h \
        ../../src/propertiesview.h \
        ../../src/rendermanager.h \
        ../../src/spinboxentity.h \
        ../../src/spinboxsliderentity.h \
        ../../src/uicolors.h \
        ../../src/uientity.h \
        ../../src/viewerstatusbar.h \
        ../../src/vulkanrenderer.h \
        ../../src/vulkanview.h \
        ../../src/vulkanwindow.h \
        ../../src/windowmanager.h \
        ../../src/writepropertiesentity.h


SOURCES += \
        main.cpp \
        ../../src/colorbutton.cpp \
        ../../src/colorbuttonentity.cpp \
        ../../src/comboboxentity.cpp \
        ../../src/connection.cpp \
        ../../src/csimage.cpp \
        ../../src/csslider.cpp \
        ../../src/fileboxentity.cpp \
        ../../src/mainwindow.cpp \
        ../../src/csmessagebox.cpp \
        ../../src/nodebase.cpp \
        ../../src/nodegraph.cpp \
        ../../src/nodegraphcontextmenu.cpp \
        ../../src/nodeinput.cpp \
        ../../src/nodeoutput.cpp \
        ../../src/nodeproperties.cpp \
        ../../src/propertiesheading.cpp \
        ../../src/propertiesview.cpp \
        ../../src/rendermanager.cpp \
        ../../src/spinboxentity.cpp \
        ../../src/spinboxsliderentity.cpp \
        ../../src/uientity.cpp \
        ../../src/viewerstatusbar.cpp \
        ../../src/vulkanrenderer.cpp \
        ../../src/vulkanview.cpp \
        ../../src/vulkanwindow.cpp \
        ../../src/windowmanager.cpp \
        ../../src/writepropertiesentity.cpp


FORMS += \
        ../../src/colorbuttonentity.ui \
        ../../src/comboboxentity.ui \
        ../../src/fileboxentity.ui \
        ../../src/mainwindow.ui \
        ../../src/nodebase.ui \
        ../../src/propertiesheading.ui \
        ../../src/spinboxentity.ui \
        ../../src/spinboxsliderentity.ui \
        ../../src/viewerstatusbar.ui \
        ../../src/writepropertiesentity.ui

linux-g++ {
    SOURCES += \
        /usr/src/googletest/src/gtest-all.cc \
        /usr/src/gmock/src/gmock-all.cc \

    INCLUDEPATH *= \
        /usr/src/googletest \
        /usr/src/gmock \
        ../../ \

    LIBS += -L /usr/lib -lOpenImageIO
}

win32-msvc* {
    SOURCES += \
        ../../../Documents/vcpkg/installed/x64-windows/src/gtest-all.cc \

    INCLUDEPATH += \
        ../../../Documents/vcpkg/installed/x64-windows/include \
        ../../../Documents/vcpkg/installed/x64-windows/ \
        ../../external/msvc2019/OpenImageIO/include \
        ../../ \

    LIBS += -L../../../external/msvc2019/OpenImageIO/lib -lOpenImageIO
}


