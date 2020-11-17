QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 6

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

#-------------------------------

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/channelselectentity.cpp \
    src/colorbutton.cpp \
    src/colorbuttonentity.cpp \
    src/colorpropertiesentity.cpp \
    src/comboboxentity.cpp \
    src/connection.cpp \
    src/csimage.cpp \
    src/csmessagebox.cpp \
    src/cssettingsbuffer.cpp \
    src/csslider.cpp \
    src/cssliderbox.cpp \
    src/fileboxentity.cpp \
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
    src/rendermanager.cpp \
    src/sizeboxentity.cpp \
    src/slidernoclick.cpp \
    src/spinboxentity.cpp \
    src/textboxentity.cpp \
    src/uientity.cpp \
    src/viewerstatusbar.cpp \
    src/vulkanrenderer.cpp \
    src/vulkanview.cpp \
    src/vulkanwindow.cpp \
    src/windowmanager.cpp \
    src/writepropertiesentity.cpp

HEADERS += \
    src/benchmark.h \
    src/channelselectentity.h \
    src/colorbutton.h \
    src/colorbuttonentity.h \
    src/colorpropertiesentity.h \
    src/comboboxentity.h \
    src/connection.h \
    src/csimage.h \
    src/csmessagebox.h \
    src/cssettingsbuffer.h \
    src/csslider.h \
    src/cssliderbox.h \
    src/fileboxentity.h \
    src/mainwindow.h \
    src/nodebase.h \
    src/nodedefinitions.h \
    src/nodegraph.h \
    src/nodegraphcontextmenu.h \
    src/nodeinput.h \
    src/nodeoutput.h \
    src/nodeproperties.h \
    src/propertiesheading.h \
    src/propertiesview.h \
    src/rendermanager.h \
    src/sizeboxentity.h \
    src/slidernoclick.h \
    src/spinboxentity.h \
    src/textboxentity.h \
    src/uicolors.h \
    src/uientity.h \
    src/viewerstatusbar.h \
    src/vulkanrenderer.h \
    src/vulkanview.h \
    src/vulkanwindow.h \
    src/windowmanager.h \
    src/writepropertiesentity.h

FORMS += \
    src/channelselectentity.ui \
    src/colorbuttonentity.ui \
    src/colorpropertiesentity.ui \
    src/comboboxentity.ui \
    src/cssliderbox.ui \
    src/fileboxentity.ui \
    src/mainwindow.ui \
    src/nodebase.ui \
    src/propertiesheading.ui \
    src/sizeboxentity.ui \
    src/spinboxentity.ui \
    src/textboxentity.ui \
    src/viewerstatusbar.ui \
    src/writepropertiesentity.ui

linux-g++ {
    INCLUDEPATH += ../Qt-Advanced-Docking-System/src

    LIBS += -L /usr/lib -lOpenImageIO
    LIBS += -L /usr/lib -lOpenColorIO
    LIBS += -L /usr/lib -lqtadvanceddocking

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
    VCPKG_INSTALL_PATH = "C:/Users/ryzen/vcpkg"
    QT_INSTALL_PATH = "C:/Qt515/5.15.1"
    MSVC_INSTALL_PATH = "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64"

    INCLUDEPATH += $$VCPKG_INSTALL_PATH/installed/x64-windows/include
    INCLUDEPATH += ../external/qtadvanceddocking/src

    CONFIG(release, debug|release) {
        LIBS += -L$$VCPKG_INSTALL_PATH/installed/x64-windows/lib -lOpenImageIO
        LIBS += -L$$VCPKG_INSTALL_PATH/installed/x64-windows/lib -lOpenColorIO
        LIBS += -L../external/qtadvanceddocking/build/lib -lqtadvanceddocking
    }
    CONFIG(debug, debug|release) {
        LIBS += -L$$VCPKG_INSTALL_PATH/installed/x64-windows/debug/lib -lOpenImageIO
        LIBS += -L$$VCPKG_INSTALL_PATH/installed/x64-windows/debug/lib -lOpenColorIO
        LIBS += -L../external/qtadvanceddocking/build/lib -lqtadvanceddockingd
    }

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release

    COPIES += dlls
    dlls.files += $$files($$MSVC_INSTALL_PATH/vcruntime140.dll)
    dlls.files += $$files($$MSVC_INSTALL_PATH/vcruntime140_1.dll)
    dlls.files += $$files($$MSVC_INSTALL_PATH/msvcp140.dll)
    dlls.files += $$files($$MSVC_INSTALL_PATH/concrt140.dll)
    dlls.files += $$files($$MSVC_INSTALL_PATH/msvcp140_1.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/opengl32sw.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Svg.dll)

    CONFIG(release, debug|release) {
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/OpenImageIO.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/OpenColorIO.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Widgets.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Gui.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Core.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/boost_filesystem-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/boost_system-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/boost_thread-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/boost_chrono-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/boost_date_time-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/zlib1.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/jpeg62.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/libpng16.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/IlmImf-2_5.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/Imath-2_5.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/Iex-2_5.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/Half-2_5.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/IlmThread-2_5.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/tiff.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/lzma.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/bin/yaml-cpp.dll)
    dlls.files += $$files($$DESTDIR/../../external/qtadvanceddocking/build/lib/qtadvanceddocking.dll)
    }

    CONFIG(debug, debug|release) {
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/OpenImageIO.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/OpenColorIO.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Widgetsd.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Guid.dll)
    dlls.files += $$files($$QT_INSTALL_PATH/msvc2019_64/bin/Qt5Cored.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/boost_filesystem-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/boost_system-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/boost_thread-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/boost_chrono-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/boost_date_time-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/zlibd1.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/jpeg62.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/libpng16d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/IlmImf-2_5_d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/Imath-2_5_d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/Iex-2_5_d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/Half-2_5_d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/IlmThread-2_5_d.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/tiffd.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/lzmad.dll)
    dlls.files += $$files($$VCPKG_INSTALL_PATH/installed/x64-windows/debug/bin/yaml-cpp.dll)
    dlls.files += $$files($$DESTDIR/../../external/qtadvanceddocking/build/lib/qtadvanceddockingd.dll)
    }
    dlls.path = $$DESTDIR

    CONFIG(release, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qdirect2d.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qminimal.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qoffscreen.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qwebgl.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qwindows.dll)
    platforms.path = $$DESTDIR/platforms

    COPIES += iconengines
    iconengines.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/iconengines/qsvgicon.dll)
    iconengines.path = $$DESTDIR/iconengines

    COPIES += styles
    styles.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/styles/qwindowsvistastyle.dll)
    styles.path = $$DESTDIR/styles
    }

    CONFIG(debug, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qdirect2dd.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qminimald.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qoffscreend.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qwebgld.dll)
    platforms.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/platforms/qwindowsd.dll)
    platforms.path = $$DESTDIR/platforms

    COPIES += iconengines
    iconengines.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/iconengines/qsvgicond.dll)
    iconengines.path = $$DESTDIR\iconengines

    COPIES += styles
    styles.files += $$files($$QT_INSTALL_PATH/msvc2019_64/plugins/styles/qwindowsvistastyled.dll)
    styles.path = $$DESTDIR/styles
    }

    COPIES += ocio
    ocio.files += $$files($$DESTDIR/../../ocio/config.ocio)
    ocio.path = $$DESTDIR/ocio

    COPIES += luts
    luts.files += $$files($$DESTDIR/../../ocio/luts/*)
    luts.path = $$DESTDIR/ocio/luts

    COPIES += license
    license.files += $$files($$DESTDIR/../../LICENSE)
    license.path = $$DESTDIR

    #COPIES += plugins
    #plugins.files += $$files($$DESTDIR/../../plugins/ofx)
    #plugins.path = $$DESTDIR/plugins

    INSTALLS += plugins
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss

