QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

CONFIG += c++17

DEFINES += \
    GTEST_LANG_CXX11

INCLUDEPATH *= \
    /usr/src/googletest \
    /usr/src/gmock \
    ../../ \

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
        /usr/src/googletest/src/gtest-all.cc \
        /usr/src/gmock/src/gmock-all.cc \
        main.cpp \
        ../../src/colorbutton.cpp \
        ../../src/colorbuttonentity.cpp \
        ../../src/comboboxentity.cpp \
        ../../src/connection.cpp \
        ../../src/csimage.cpp \
        ../../src/csslider.cpp \
        ../../src/fileboxentity.cpp \
        ../../src/mainwindow.cpp \
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
    LIBS += -L /usr/lib -lOpenImageIO
}


