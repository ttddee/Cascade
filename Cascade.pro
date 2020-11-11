QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 5

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
    src/comboboxentity.cpp \
    src/connection.cpp \
    src/csimage.cpp \
    src/csmessagebox.cpp \
    src/cssettingsbuffer.cpp \
    src/csslider.cpp \
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
    src/spinboxentity.cpp \
    src/spinboxsliderentity.cpp \
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
    src/comboboxentity.h \
    src/connection.h \
    src/csimage.h \
    src/csmessagebox.h \
    src/cssettingsbuffer.h \
    src/csslider.h \
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
    src/spinboxentity.h \
    src/spinboxsliderentity.h \
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
    src/comboboxentity.ui \
    src/fileboxentity.ui \
    src/mainwindow.ui \
    src/nodebase.ui \
    src/propertiesheading.ui \
    src/spinboxentity.ui \
    src/spinboxsliderentity.ui \
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
    INCLUDEPATH += ../external/msvc2019/Other/include
    INCLUDEPATH += ../external/msvc2019/OpenImageIO/include
    INCLUDEPATH += ../external/msvc2019/OpenColorIO/include
    INCLUDEPATH += ../external/msvc2019/AdvancedDocking/include

    LIBS += -L../external/msvc2019/OpenImageIO/lib -lOpenImageIO
    LIBS += -L../external/msvc2019/OpenColorIO/lib -lOpenColorIO
    LIBS += -L../external/msvc2019/AdvancedDocking/lib -lqtadvanceddocking

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release

    COPIES += dlls
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/OpenImageIO.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenColorIO/bin/OpenColorIO.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/vcruntime140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/vcruntime140_1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/msvcp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/concrt140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/vcomp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/msvcp140_1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/opengl32sw.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Svg.dll)

    CONFIG(release, debug|release) {
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Widgets.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Gui.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Core.dll)
    }

    CONFIG(debug, debug|release) {
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Widgetsd.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Guid.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Cored.dll)
    }

    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/IlmImf-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/IlmThread-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/Imath-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/Iex-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/Half-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/raw_r.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/jpeg62.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/libpng16.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/lzma.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/lcms2.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/jasper.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/zlib1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Boost/boost_filesystem-vc141-mt-x64-1_66.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Boost/boost_system-vc141-mt-x64-1_66.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Boost/boost_thread-vc141-mt-x64-1_66.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Boost/boost_chrono-vc141-mt-x64-1_66.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Boost/boost_date_time-vc141-mt-x64-1_66.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/AdvancedDocking/lib/qtadvanceddocking.dll)
    dlls.path = $$DESTDIR

    CONFIG(release, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$DESTDIR/../../external/msvc2019/Qt/platforms/release/*)
    platforms.path = $$DESTDIR\platforms
    }

    CONFIG(debug, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$DESTDIR/../../external/msvc2019/Qt/platforms/debug/*)
    platforms.path = $$DESTDIR\platforms
    }

    COPIES += iconengines
    iconengines.files += $$files($$DESTDIR/../../external/msvc2019/Qt/iconengines/*)
    iconengines.path = $$DESTDIR\iconengines

    COPIES += imageformats
    imageformats.files += $$files($$DESTDIR/../../external/msvc2019/Qt/imageformats/*)
    imageformats.path = $$DESTDIR/imageformats

    COPIES += styles
    styles.files += $$files($$DESTDIR/../../external/msvc2019/Qt/styles/*)
    styles.path = $$DESTDIR/styles

    COPIES += translations
    translations.files += $$files($$DESTDIR/../../external/msvc2019/Qt/translations/*)
    translations.path = $$DESTDIR/translations

    COPIES += ocio
    ocio.files += $$files($$DESTDIR/../../ocio/config.ocio)
    ocio.path = $$DESTDIR/ocio

    COPIES += luts
    luts.files += $$files($$DESTDIR/../../ocio/luts/*)
    luts.path = $$DESTDIR/ocio/luts

    COPIES += license
    license.files += $$files($$DESTDIR/../../LICENSE)
    license.path = $$DESTDIR
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss

