QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 7

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

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
    src/gmic/FiltersModel.cpp \
    src/gmic/FiltersModelReader.cpp \
    src/gmic/FiltersVisibilityMap.cpp \
    src/gmic/Globals.cpp \
    src/gmic/GmicStdlib.cpp \
    src/gmic/HtmlTranslator.cpp \
    src/gmic/InputOutputState.cpp \
    src/gmic/ParametersCache.cpp \
    src/gmic/Utils.cpp \
    src/gmichelper.cpp \
    src/log.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nodebase.cpp \
    src/nodegraph.cpp \
    src/nodegraphcontextmenu.cpp \
    src/nodeinput.cpp \
    src/nodeoutput.cpp \
    src/nodeproperties.cpp \
    src/propertiesheading.cpp \
    src/propertiesview.cpp \
    src/renderer/csimage.cpp \
    src/renderer/cssettingsbuffer.cpp \
    src/renderer/objectmanager.cpp \
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
    src/uientities/gmicpropertiesentity.cpp \
    src/uientities/lineeditentity.cpp \
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
    src/gmic/FiltersModel.h \
    src/gmic/FiltersModelReader.h \
    src/gmic/FiltersVisibilityMap.h \
    src/gmic/Globals.h \
    src/gmic/GmicDefinitions.h \
    src/gmic/GmicStdlib.h \
    src/gmic/HtmlTranslator.h \
    src/gmic/InputOutputState.h \
    src/gmic/ParametersCache.h \
    src/gmic/Utils.h \
    src/gmichelper.h \
    src/log.h \
    src/mainwindow.h \
    src/multithreading.h \
    src/nodebase.h \
    src/nodedefinitions.h \
    src/nodegraph.h \
    src/nodegraphcontextmenu.h \
    src/nodeinput.h \
    src/nodeoutput.h \
    src/nodeproperties.h \
    src/propertiesheading.h \
    src/propertiesview.h \
    src/renderer/csimage.h \
    src/renderer/cssettingsbuffer.h \
    src/renderer/objectmanager.h \
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
    src/uientities/gmicpropertiesentity.h \
    src/uientities/lineeditentity.h \
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
    src/uientities/gmicpropertiesentity.ui \
    src/uientities/lineeditentity.ui \
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

win32-g++ {
    DEPENDENCY_ROOT = C:/msys64/mingw64
    QT_ROOT = C:\msys64\home\till\qt515\5.15.1\mingw81_64
    TEMP_ROOT = C:/msys64/home/till/temp

    LIBS += -L$$DEPENDENCY_ROOT/lib -llibOpenImageIO
    LIBS += -L$$DEPENDENCY_ROOT/lib -llibOpenColorIO
    LIBS += -L$$DEPENDENCY_ROOT/lib -llibgmic
    LIBS += -L$$DEPENDENCY_ROOT/lib -llibtbb

    INCLUDEPATH += $$DEPENDENCY_ROOT/include

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release

#    COPIES += ocio
#    ocio.files += $$files(ocio/config.ocio)
#    ocio.path = $$DESTDIR/ocio

#    COPIES += luts
#    luts.files += $$files(ocio/luts/*)
#    luts.path = $$DESTDIR/ocio/luts

    COPIES += license
    license.files += $$files(LICENSE)
    license.path = $$DESTDIR

    COPIES += platforms
    platforms.files += $$files($$QT_ROOT/plugins/platforms/qdirect2d.dll)
    platforms.files += $$files($$QT_ROOT/plugins/platforms/qminimal.dll)
    platforms.files += $$files($$QT_ROOT/plugins/platforms/qoffscreen.dll)
    platforms.files += $$files($$QT_ROOT/plugins/platforms/qwindows.dll)
    platforms.path = $$DESTDIR/platforms

    COPIES += iconengines
    iconengines.files += $$files($$QT_ROOT/plugins/iconengines/qsvgicon.dll)
    iconengines.path = $$DESTDIR/iconengines

    COPIES += styles
    styles.files += $$files($$QT_ROOT/plugins/styles/qwindowsvistastyle.dll)
    styles.path = $$DESTDIR/styles

    COPIES += dlls
    dlls.files += $$files($$QT_ROOT/bin/Qt5Svg.dll)
    dlls.files += $$files($$QT_ROOT/bin/Qt5Widgets.dll)
    dlls.files += $$files($$QT_ROOT/bin/Qt5Gui.dll)
    dlls.files += $$files($$QT_ROOT/bin/Qt5Core.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libOpenImageIO.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libOpenImageIO_Util.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libOpenColorIO.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libboost_filesystem-mt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libboost_system-mt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libboost_thread-mt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libboost_chrono-mt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libboost_date_time-mt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libzlib1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libjpeg62.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblibpng16.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libIlmImf-2_5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libImath-2_5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libIex-2_5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libHalf-2_5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libIlmThread-2_5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libtiff-5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblzma.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libyaml-cpp.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/tbb.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgmic.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgcc_s_seh-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libstdc++-6.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libtinyxml-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libwinpthread-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libfftw3-3.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libGraphicsMagick++-12.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libGraphicsMagick-3.dll)
    dlls.files += $$files($$TEMP_ROOT/libopencv_core440.dll)
    dlls.files += $$files($$TEMP_ROOT/libopencv_videoio440.dll)
    dlls.files += $$files($$TEMP_ROOT/libopencv_imgproc440.dll)
    dlls.files += $$files($$TEMP_ROOT/libopencv_imgcodecs440.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgomp-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libcurl-4.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libjpeg-8.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libpng16-16.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/zlib1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libharfbuzz-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/avcodec-58.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/avformat-58.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/avutil-56.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libField3D-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgif-7.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libhdf5-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libheif-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencv_core450.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencv_videoio450.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencv_imgproc450.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencv_imgcodecs450.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libbrotlidec.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopenblas.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libcrypto-1_1-x64.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libidn2-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libnghttp2-14.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libpsl-5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libssh2-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libssl-1_1-x64.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libzstd.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopenjp2-7.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopenvdb.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libPtex.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libpugixml.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libraw-20.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libsquish.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/swscale-5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libwebp-7.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libfreetype-6.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libaom.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libcelt0-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libdav1d.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgsm.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libiconv-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libbluray-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgnutls-30.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libbz2-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblzma-5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libmodeplug-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/librtmp-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libsrt-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libxml2-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libmfx.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libmp3lame-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencore-amrnb-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopencore-amrwb-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libspeex-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libopus-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libtheoradec-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libtheoraenc-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libvorbisenc-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libvorbis-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libwavpack-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libvpx-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libwebpmux-3.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libx264-159.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/swresample-3.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libde265-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libblosc.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblcms2-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libjasper-4.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libhogweed-6.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgmp-10.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libnettle-8.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libtasn1-6.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libp11-kit-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libbrotlicommon.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libogg-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgraphite2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libglib-2.0-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libiconv-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libx265.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgsm.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/xvidcore.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libszip.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libintl-8.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libffi-7.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libpcre-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libgfortran-5.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libquadmath-0.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libltdl-7.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libmodplug-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libsrt.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libmfx-1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libunistring-2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblz4.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/bin/libvulkan-1.dll)
    dlls.path = $$DESTDIR
}

RESOURCES += \
    ads.qrc \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss

