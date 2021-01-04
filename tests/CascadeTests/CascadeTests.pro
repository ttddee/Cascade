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
        ../../src/csmessagebox.h \
        ../../src/docking/DockAreaTabBar.h \
        ../../src/docking/DockAreaTitleBar.h \
        ../../src/docking/DockAreaTitleBar_p.h \
        ../../src/docking/DockAreaWidget.h \
        ../../src/docking/DockComponentsFactory.h \
        ../../src/docking/DockContainerWidget.h \
        ../../src/docking/DockFocusController.h \
        ../../src/docking/DockManager.h \
        ../../src/docking/DockOverlay.h \
        ../../src/docking/DockSplitter.h \
        ../../src/docking/DockWidget.h \
        ../../src/docking/DockWidgetTab.h \
        ../../src/docking/DockingStateReader.h \
        ../../src/docking/ElidingLabel.h \
        ../../src/docking/FloatingDockContainer.h \
        ../../src/docking/FloatingDragPreview.h \
        ../../src/docking/IconProvider.h \
        ../../src/docking/ads_globals.h \
        ../../src/docking/linux/FloatingWidgetTitleBar.h \
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
        ../../src/log.h \
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
        ../../src/renderer/csimage.h \
        ../../src/renderer/cssettingsbuffer.h \
        ../../src/renderer/objectmanager.h \
        ../../src/renderer/vulkanrenderer.h \
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
        ../../src/uientities/resizepropertiesentity.h \
        ../../src/uientities/separatorentity.h \
        ../../src/uientities/sizeboxentity.h \
        ../../src/uientities/spinboxentity.h \
        ../../src/uientities/textboxentity.h \
        ../../src/uientities/textbrowserentity.h \
        ../../src/uientities/uientity.h \
        ../../src/uientities/writepropertiesentity.h \
        ../../src/viewerstatusbar.h \
        ../../src/vulkanview.h \
        ../../src/vulkanwindow.h \
        ../../src/windowmanager.h \
        tst_resizepropertiesentitytests.h


SOURCES += \
        main.cpp \
        ../../src/colorbutton.cpp \
        ../../src/connection.cpp \
        ../../src/csmessagebox.cpp \
        ../../src/docking/DockAreaTabBar.cpp \
        ../../src/docking/DockAreaTitleBar.cpp \
        ../../src/docking/DockAreaWidget.cpp \
        ../../src/docking/DockComponentsFactory.cpp \
        ../../src/docking/DockContainerWidget.cpp \
        ../../src/docking/DockFocusController.cpp \
        ../../src/docking/DockManager.cpp \
        ../../src/docking/DockOverlay.cpp \
        ../../src/docking/DockSplitter.cpp \
        ../../src/docking/DockWidget.cpp \
        ../../src/docking/DockWidgetTab.cpp \
        ../../src/docking/DockingStateReader.cpp \
        ../../src/docking/ElidingLabel.cpp \
        ../../src/docking/FloatingDockContainer.cpp \
        ../../src/docking/FloatingDragPreview.cpp \
        ../../src/docking/IconProvider.cpp \
        ../../src/docking/ads_globals.cpp \
        ../../src/docking/linux/FloatingWidgetTitleBar.cpp \
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
        ../../src/log.cpp \
        ../../src/mainwindow.cpp \
        ../../src/nodebase.cpp \
        ../../src/nodegraph.cpp \
        ../../src/nodegraphcontextmenu.cpp \
        ../../src/nodeinput.cpp \
        ../../src/nodeoutput.cpp \
        ../../src/nodeproperties.cpp \
        ../../src/propertiesheading.cpp \
        ../../src/propertiesview.cpp \
        ../../src/renderer/csimage.cpp \
        ../../src/renderer/cssettingsbuffer.cpp \
        ../../src/renderer/objectmanager.cpp \
        ../../src/renderer/vulkanrenderer.cpp \
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
        ../../src/uientities/resizepropertiesentity.cpp \
        ../../src/uientities/separatorentity.cpp \
        ../../src/uientities/sizeboxentity.cpp \
        ../../src/uientities/spinboxentity.cpp \
        ../../src/uientities/textboxentity.cpp \
        ../../src/uientities/textbrowserentity.cpp \
        ../../src/uientities/uientity.cpp \
        ../../src/uientities/writepropertiesentity.cpp \
        ../../src/viewerstatusbar.cpp \
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
        ../../src/uientities/resizepropertiesentity.ui \
        ../../src/uientities/separatorentity.ui \
        ../../src/uientities/sizeboxentity.ui \
        ../../src/uientities/spinboxentity.ui \
        ../../src/uientities/textboxentity.ui \
        ../../src/uientities/textbrowserentity.ui \
        ../../src/uientities/writepropertiesentity.ui \
        ../../src/viewerstatusbar.ui

RESOURCES += \
    ../../ads.qrc \
    ../../resources.qrc

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


