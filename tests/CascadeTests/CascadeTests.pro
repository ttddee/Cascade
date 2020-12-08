QT       += core gui testlib

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
        tst_cssliderboxtests.h \
        tst_nodebasetests.h \
        ../../src/benchmark.h \
        ../../src/colorbutton.h \
        ../../src/connection.h \
        ../../src/renderer/csimage.h \
        ../../src/csmessagebox.h \
        ../../src/renderer/cssettingsbuffer.h \
        ../../src/gmic/FilterTextTranslator.h \
        ../../src/gmic/FiltersModel.h \
        ../../src/gmic/FiltersModelReader.h \
        ../../src/gmic/FiltersVisibilityMap.h \
        ../../src/gmic/Globals.h \
        ../../src/gmic/GmicDefinitions.h \
        ../../src/gmic/GmicStdlib.h \
        ../../src/gmic/HtmlTranslator.h \
        ../../src/gmic/InputOutputState.h \
        ../../src/gmic/ParametersCache.h \
        ../../src/gmic/Utils.h \
        ../../src/gmichelper.h \
        ../../src/mainwindow.h \
        ../../src/multithreading.h \
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
        ../../src/slidernoclick.h \
        ../../src/uicolors.h \
        ../../src/uientities/channelselectentity.h \
        ../../src/uientities/checkboxentity.h \
        ../../src/uientities/colorbuttonentity.h \
        ../../src/uientities/colorpropertiesentity.h \
        ../../src/uientities/comboboxentity.h \
        ../../src/uientities/cssliderboxentity.h \
        ../../src/uientities/fileboxentity.h \
        ../../src/uientities/folderboxentity.h \
        ../../src/uientities/gmicpropertiesentity.h \
        ../../src/uientities/lineeditentity.h \
        ../../src/uientities/separatorentity.h \
        ../../src/uientities/sizeboxentity.h \
        ../../src/uientities/spinboxentity.h \
        ../../src/uientities/textboxentity.h \
        ../../src/uientities/textbrowserentity.h \
        ../../src/uientities/uientity.h \
        ../../src/uientities/writepropertiesentity.h \
        ../../src/viewerstatusbar.h \
        ../../src/renderer/vulkanrenderer.h \
        ../../src/vulkanview.h \
        ../../src/vulkanwindow.h \
        ../../src/windowmanager.h


SOURCES += \
        main.cpp \
        ../../src/colorbutton.cpp \
        ../../src/connection.cpp \
        ../../src/renderer/csimage.cpp \
        ../../src/csmessagebox.cpp \
        ../../src/renderer/cssettingsbuffer.cpp \
        ../../src/gmic/FilterTextTranslator.cpp \
        ../../src/gmic/FiltersModel.cpp \
        ../../src/gmic/FiltersModelReader.cpp \
        ../../src/gmic/FiltersVisibilityMap.cpp \
        ../../src/gmic/Globals.cpp \
        ../../src/gmic/GmicStdlib.cpp \
        ../../src/gmic/HtmlTranslator.cpp \
        ../../src/gmic/InputOutputState.cpp \
        ../../src/gmic/ParametersCache.cpp \
        ../../src/gmic/Utils.cpp \
        ../../src/gmichelper.cpp \
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
        ../../src/slidernoclick.cpp \
        ../../src/uientities/channelselectentity.cpp \
        ../../src/uientities/checkboxentity.cpp \
        ../../src/uientities/colorbuttonentity.cpp \
        ../../src/uientities/colorpropertiesentity.cpp \
        ../../src/uientities/comboboxentity.cpp \
        ../../src/uientities/cssliderboxentity.cpp \
        ../../src/uientities/fileboxentity.cpp \
        ../../src/uientities/folderboxentity.cpp \
        ../../src/uientities/gmicpropertiesentity.cpp \
        ../../src/uientities/lineeditentity.cpp \
        ../../src/uientities/separatorentity.cpp \
        ../../src/uientities/sizeboxentity.cpp \
        ../../src/uientities/spinboxentity.cpp \
        ../../src/uientities/textboxentity.cpp \
        ../../src/uientities/textbrowserentity.cpp \
        ../../src/uientities/uientity.cpp \
        ../../src/uientities/writepropertiesentity.cpp \
        ../../src/viewerstatusbar.cpp \
        ../../src/renderer/vulkanrenderer.cpp \
        ../../src/vulkanview.cpp \
        ../../src/vulkanwindow.cpp \
        ../../src/windowmanager.cpp


FORMS += \
        ../../src/mainwindow.ui \
        ../../src/nodebase.ui \
        ../../src/propertiesheading.ui \
        ../../src/uientities/channelselectentity.ui \
        ../../src/uientities/checkboxentity.ui \
        ../../src/uientities/colorbuttonentity.ui \
        ../../src/uientities/colorpropertiesentity.ui \
        ../../src/uientities/comboboxentity.ui \
        ../../src/uientities/cssliderboxentity.ui \
        ../../src/uientities/fileboxentity.ui \
        ../../src/uientities/folderboxentity.ui \
        ../../src/uientities/gmicpropertiesentity.ui \
        ../../src/uientities/lineeditentity.ui \
        ../../src/uientities/separatorentity.ui \
        ../../src/uientities/sizeboxentity.ui \
        ../../src/uientities/spinboxentity.ui \
        ../../src/uientities/textboxentity.ui \
        ../../src/uientities/textbrowserentity.ui \
        ../../src/uientities/writepropertiesentity.ui \
        ../../src/viewerstatusbar.ui

linux-g++ {
    SOURCES += \
        /usr/src/gtest/src/gtest-all.cc \
        /usr/src/gmock/src/gmock-all.cc \

    INCLUDEPATH *= \
        /usr/src/googletest \
        /usr/src/gtest \
        /usr/src/gmock \
        ../../ \

    LIBS += -L /usr/lib -lOpenImageIO
    LIBS += -L /usr/lib -lOpenColorIO
    LIBS += -L /usr/lib -lqtadvanceddocking
    LIBS += -L /usr/lib -lgmic
    LIBS += -L /usr/lib -ltbb

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD

    COPIES += ocio
    ocio.files += $$files(ocio/config.ocio)
    ocio.path = $$DESTDIR/ocio

    COPIES += luts
    luts.files += $$files(ocio/luts/*)
    luts.path = $$DESTDIR/ocio/luts
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


