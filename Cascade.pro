QT       += core gui widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS_RELEASE -= -O2

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 7

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00

#-------------------------------

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/colorbutton.cpp \
    src/connection.cpp \
    src/csmessagebox.cpp \
    src/docking/DockAreaTabBar.cpp \
    src/docking/DockAreaTitleBar.cpp \
    src/docking/DockAreaWidget.cpp \
    src/docking/DockComponentsFactory.cpp \
    src/docking/DockContainerWidget.cpp \
    src/docking/DockFocusController.cpp \
    src/docking/DockManager.cpp \
    src/docking/DockOverlay.cpp \
    src/docking/DockSplitter.cpp \
    src/docking/DockWidget.cpp \
    src/docking/DockWidgetTab.cpp \
    src/docking/DockingStateReader.cpp \
    src/docking/ElidingLabel.cpp \
    src/docking/FloatingDockContainer.cpp \
    src/docking/FloatingDragPreview.cpp \
    src/docking/IconProvider.cpp \
    src/docking/ads_globals.cpp \
    src/docking/linux/FloatingWidgetTitleBar.cpp \
    src/log.cpp \
    src/main.cpp \
    src/mainmenu.cpp \
    src/mainwindow.cpp \
    src/nodebase.cpp \
    src/nodegraph.cpp \
    src/nodegraphcontextmenu.cpp \
    src/nodeinput.cpp \
    src/nodeoutput.cpp \
    src/nodeproperties.cpp \
    src/projectmanager.cpp \
    src/propertiesheading.cpp \
    src/propertiesview.cpp \
    src/renderer/csimage.cpp \
    src/renderer/cssettingsbuffer.cpp \
    src/renderer/vulkanrenderer.cpp \
    src/rendermanager.cpp \
    src/slidernoclick.cpp \
    src/uientities/channelselectentity.cpp \
    src/uientities/checkboxentity.cpp \
    src/uientities/colorbuttonentity.cpp \
    src/uientities/colorpropertiesentity.cpp \
    src/uientities/comboboxentity.cpp \
    src/uientities/cssliderboxentity.cpp \
    src/uientities/fileboxentity.cpp \
    src/uientities/folderboxentity.cpp \
    src/uientities/lineeditentity.cpp \
    src/uientities/resizepropertiesentity.cpp \
    src/uientities/separatorentity.cpp \
    src/uientities/sizeboxentity.cpp \
    src/uientities/spinboxentity.cpp \
    src/uientities/textboxentity.cpp \
    src/uientities/textbrowserentity.cpp \
    src/uientities/uientity.cpp \
    src/uientities/writepropertiesentity.cpp \
    src/viewerstatusbar.cpp \
    src/vulkanview.cpp \
    src/vulkanwindow.cpp \
    src/windowmanager.cpp

HEADERS += \
    src/benchmark.h \
    src/colorbutton.h \
    src/connection.h \
    src/csmessagebox.h \
    src/docking/DockAreaTabBar.h \
    src/docking/DockAreaTitleBar.h \
    src/docking/DockAreaTitleBar_p.h \
    src/docking/DockAreaWidget.h \
    src/docking/DockComponentsFactory.h \
    src/docking/DockContainerWidget.h \
    src/docking/DockFocusController.h \
    src/docking/DockManager.h \
    src/docking/DockOverlay.h \
    src/docking/DockSplitter.h \
    src/docking/DockWidget.h \
    src/docking/DockWidgetTab.h \
    src/docking/DockingStateReader.h \
    src/docking/ElidingLabel.h \
    src/docking/FloatingDockContainer.h \
    src/docking/FloatingDragPreview.h \
    src/docking/IconProvider.h \
    src/docking/ads_globals.h \
    src/docking/linux/FloatingWidgetTitleBar.h \
    src/log.h \
    src/mainmenu.h \
    src/mainwindow.h \
    src/multithreading.h \
    src/nodebase.h \
    src/nodedefinitions.h \
    src/nodegraph.h \
    src/nodegraphcontextmenu.h \
    src/nodeinput.h \
    src/nodeoutput.h \
    src/nodeproperties.h \
    src/projectmanager.h \
    src/propertiesheading.h \
    src/propertiesview.h \
    src/renderer/csimage.h \
    src/renderer/cssettingsbuffer.h \
    src/renderer/vulkanrenderer.h \
    src/rendermanager.h \
    src/slidernoclick.h \
    src/uicolors.h \
    src/uientities/channelselectentity.h \
    src/uientities/checkboxentity.h \
    src/uientities/colorbuttonentity.h \
    src/uientities/colorpropertiesentity.h \
    src/uientities/comboboxentity.h \
    src/uientities/cssliderboxentity.h \
    src/uientities/fileboxentity.h \
    src/uientities/folderboxentity.h \
    src/uientities/lineeditentity.h \
    src/uientities/resizepropertiesentity.h \
    src/uientities/separatorentity.h \
    src/uientities/sizeboxentity.h \
    src/uientities/spinboxentity.h \
    src/uientities/textboxentity.h \
    src/uientities/textbrowserentity.h \
    src/uientities/uientity.h \
    src/uientities/writepropertiesentity.h \
    src/viewerstatusbar.h \
    src/vulkanview.h \
    src/vulkanwindow.h \
    src/windowmanager.h

FORMS += \
    src/mainwindow.ui \
    src/nodebase.ui \
    src/propertiesheading.ui \
    src/uientities/channelselectentity.ui \
    src/uientities/checkboxentity.ui \
    src/uientities/colorbuttonentity.ui \
    src/uientities/colorpropertiesentity.ui \
    src/uientities/comboboxentity.ui \
    src/uientities/cssliderboxentity.ui \
    src/uientities/fileboxentity.ui \
    src/uientities/folderboxentity.ui \
    src/uientities/lineeditentity.ui \
    src/uientities/resizepropertiesentity.ui \
    src/uientities/separatorentity.ui \
    src/uientities/sizeboxentity.ui \
    src/uientities/spinboxentity.ui \
    src/uientities/textboxentity.ui \
    src/uientities/textbrowserentity.ui \
    src/uientities/writepropertiesentity.ui \
    src/viewerstatusbar.ui

linux-g++ {
    LIBS += -L /usr/lib -lOpenImageIO
    #LIBS += -L /usr/lib -lOpenColorIO
    LIBS += -L /usr/lib -lgmic
    LIBS += -L /usr/lib -ltbb

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD

#    COPIES += ocio
#    ocio.files += $$files(ocio/config.ocio)
#    ocio.path = $$DESTDIR/ocio

#    COPIES += luts
#    luts.files += $$files(ocio/luts/*)
#    luts.path = $$DESTDIR/ocio/luts
}

win32-msvc* {
    DEPENDENCY_ROOT = vcpkg_installed/x64-windows
    LIB_ROOT = ../vcpkg_installed/x64-windows
    QT_ROOT = C:/Qt515/5.15.0/msvc2019_64

    INCLUDEPATH += $$DEPENDENCY_ROOT/include

    COPIES += dlls
    COPIES += platforms

    CONFIG(debug, debug|release) {
        DESTDIR = $$OUT_PWD/debug

        # Debug Libs
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenImageIO_d
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenImageIO_Util_d
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenColorIO
        LIBS += -L$$LIB_ROOT/debug/lib -ltbb_debug

        # Debug DLLs
        dlls.files += $$files($$QT_ROOT/bin/Qt5Svgd.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Widgetsd.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Guid.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Cored.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/tbb_debug.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenImageIO_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenImageIO_Util_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenColorIO_2_1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_filesystem-vc142-mt-gd-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_system-vc142-mt-gd-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_thread-vc142-mt-gd-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_chrono-vc142-mt-gd-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_date_time-vc142-mt-gd-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/jasperd.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/raw_rd.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/openjp2.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/heif.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/libpng16d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/zlibd1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/lzmad.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/tiffd.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/IlmImf-2_5_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/Imath-2_5_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/Imath-3_1_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/Iex-2_5_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/Half-2_5_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/IlmThread-2_5_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/jpeg62.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/gif.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenColorIO_2_0.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/squishd.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/libde265.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/libx265.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/lcms2.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/yaml-cppd.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/libexpatd.dll)

        platforms.files += $$files($$QT_ROOT/plugins/platforms/qdirect2dd.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qminimald.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qoffscreend.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qwindowsd.dll)
    }
    CONFIG(release, debug|release) {
        DESTDIR = $$OUT_PWD/release

        # Release Libs
        LIBS += -L$$LIB_ROOT/lib -lOpenImageIO
        LIBS += -L$$LIB_ROOT/lib -lOpenImageIO_Util
        LIBS += -L$$LIB_ROOT/lib -lOpenColorIO
        LIBS += -L$$LIB_ROOT/lib -ltbb

        # Release DLLs
        dlls.files += $$files($$QT_ROOT/bin/Qt5Svg.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Widgets.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Gui.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Core.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/tbb.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenImageIO.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenImageIO_Util.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenColorIO_2_1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_filesystem-vc142-mt-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_system-vc142-mt-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_thread-vc142-mt-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_chrono-vc142-mt-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_date_time-vc142-mt-x64-1_77.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/jasper.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/raw_r.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/openjp2.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/heif.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/libpng16.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/zlib1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/lzma.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/tiff.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/IlmImf-2_5.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/Imath-2_5.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/Imath-3_1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/Iex-2_5.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/Half-2_5.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/IlmThread-2_5.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/jpeg62.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/gif.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenColorIO_2_0.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/squish.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/libde265.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/libx265.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/lcms2.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/yaml-cpp.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/libexpat.dll)

        platforms.files += $$files($$QT_ROOT/plugins/platforms/qdirect2d.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qminimal.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qoffscreen.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qwindows.dll)
    }

    dlls.path = $$DESTDIR
    platforms.path = $$DESTDIR/platforms

    COPIES += ocio
    ocio.files += $$files(ocio/config.ocio)
    ocio.path = $$DESTDIR/ocio

    COPIES += luts
    luts.files += $$files(ocio/luts/*)
    luts.path = $$DESTDIR/ocio/luts

    COPIES += license
    license.files += $$files(LICENSE)
    license.path = $$DESTDIR

    COPIES += iconengines
    iconengines.files += $$files($$QT_ROOT/plugins/iconengines/qsvgicon.dll)
    iconengines.path = $$DESTDIR/iconengines

    COPIES += styles
    styles.files += $$files($$QT_ROOT/plugins/styles/qwindowsvistastyle.dll)
    styles.path = $$DESTDIR/styles


}

RESOURCES += \
    ads.qrc \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss

