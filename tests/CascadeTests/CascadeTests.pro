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
        tst_cssliderboxtests.h \
        tst_nodebasetests.h \
        tst_testtest.h \
        ../../src/benchmark.h \
        ../../src/channelselectentity.h \
        ../../src/checkboxentity.h \
        ../../src/colorbutton.h \
        ../../src/colorbuttonentity.h \
        ../../src/colorpropertiesentity.h \
        ../../src/comboboxentity.h \
        ../../src/connection.h \
        ../../src/csimage.h \
        ../../src/csmessagebox.h \
        ../../src/cssettingsbuffer.h \
        ../../src/csslider.h \
        ../../src/cssliderbox.h \
        ../../src/fileboxentity.h \
        ../../src/folderboxentity.h \
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
        ../../src/gmicpropertiesentity.h \
        ../../src/lineeditentity.h \
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
        ../../src/separatorentity.h \
        ../../src/sizeboxentity.h \
        ../../src/slidernoclick.h \
        ../../src/spinboxentity.h \
        ../../src/textboxentity.h \
        ../../src/textbrowserentity.h \
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
        ../../src/channelselectentity.cpp \
        ../../src/checkboxentity.cpp \
        ../../src/colorbutton.cpp \
        ../../src/colorbuttonentity.cpp \
        ../../src/colorpropertiesentity.cpp \
        ../../src/comboboxentity.cpp \
        ../../src/connection.cpp \
        ../../src/csimage.cpp \
        ../../src/csmessagebox.cpp \
        ../../src/cssettingsbuffer.cpp \
        ../../src/csslider.cpp \
        ../../src/cssliderbox.cpp \
        ../../src/fileboxentity.cpp \
        ../../src/folderboxentity.cpp \
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
        ../../src/gmicpropertiesentity.cpp \
        ../../src/lineeditentity.cpp \
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
        ../../src/separatorentity.cpp \
        ../../src/sizeboxentity.cpp \
        ../../src/slidernoclick.cpp \
        ../../src/spinboxentity.cpp \
        ../../src/textboxentity.cpp \
        ../../src/textbrowserentity.cpp \
        ../../src/uientity.cpp \
        ../../src/viewerstatusbar.cpp \
        ../../src/vulkanrenderer.cpp \
        ../../src/vulkanview.cpp \
        ../../src/vulkanwindow.cpp \
        ../../src/windowmanager.cpp \
        ../../src/writepropertiesentity.cpp


FORMS += \
        ../../src/channelselectentity.ui \
        ../../src/checkboxentity.ui \
        ../../src/colorbuttonentity.ui \
        ../../src/colorpropertiesentity.ui \
        ../../src/comboboxentity.ui \
        ../../src/cssliderbox.ui \
        ../../src/fileboxentity.ui \
        ../../src/folderboxentity.ui \
        ../../src/gmicpropertiesentity.ui \
        ../../src/lineeditentity.ui \
        ../../src/mainwindow.ui \
        ../../src/nodebase.ui \
        ../../src/propertiesheading.ui \
        ../../src/separatorentity.ui \
        ../../src/sizeboxentity.ui \
        ../../src/spinboxentity.ui \
        ../../src/textboxentity.ui \
        ../../src/textbrowserentity.ui \
        ../../src/viewerstatusbar.ui \
        ../../src/writepropertiesentity.ui

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


