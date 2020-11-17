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
    src/ofx/CsOfxClipInstance.cpp \
    src/ofx/CsOfxEffectInstance.cpp \
    src/ofx/CsOfxHostDescriptor.cpp \
    src/ofx/CsOfxParamInstance.cpp \
    src/ofx/ofxhBinary.cpp \
    src/ofx/ofxhClip.cpp \
    src/ofx/ofxhHost.cpp \
    src/ofx/ofxhImageEffect.cpp \
    src/ofx/ofxhImageEffectAPI.cpp \
    src/ofx/ofxhInteract.cpp \
    src/ofx/ofxhMemory.cpp \
    src/ofx/ofxhParam.cpp \
    src/ofx/ofxhPluginAPICache.cpp \
    src/ofx/ofxhPluginCache.cpp \
    src/ofx/ofxhPropertySuite.cpp \
    src/ofx/ofxhUtilities.cpp \
    src/ofxmanager.cpp \
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
    src/ofx/CsOfxClipInstance.h \
    src/ofx/CsOfxEffectInstance.h \
    src/ofx/CsOfxHostDescriptor.h \
    src/ofx/CsOfxParamInstance.h \
    src/ofx/ofxhBinary.h \
    src/ofx/ofxhClip.h \
    src/ofx/ofxhHost.h \
    src/ofx/ofxhImageEffect.h \
    src/ofx/ofxhImageEffectAPI.h \
    src/ofx/ofxhInteract.h \
    src/ofx/ofxhMemory.h \
    src/ofx/ofxhParam.h \
    src/ofx/ofxhPluginAPICache.h \
    src/ofx/ofxhPluginCache.h \
    src/ofx/ofxhProgress.h \
    src/ofx/ofxhPropertySuite.h \
    src/ofx/ofxhTimeLine.h \
    src/ofx/ofxhUtilities.h \
    src/ofx/ofxhXml.h \
    src/ofxmanager.h \
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
    INCLUDEPATH += ../external/msvc2019/Other/include
    INCLUDEPATH += ../../vcpkg/installed/x64-windows/include
    INCLUDEPATH += ../external/msvc2019/OpenColorIO/include
    INCLUDEPATH += ../external/msvc2019/AdvancedDocking/include
    INCLUDEPATH += ../external/msvc2019/ofx/include
    INCLUDEPATH += ../external/msvc2019/expat/include

    LIBS += -L../external/msvc2019/OpenColorIO/lib -lOpenColorIO
    LIBS += -L../external/msvc2019/expat/lib -llibexpat

    CONFIG(release, debug|release) {
        LIBS += -L../external/msvc2019/AdvancedDocking/lib -lqtadvanceddocking
        LIBS += -L../../vcpkg/installed/x64-windows/lib -lOpenImageIO
    }
    CONFIG(debug, debug|release) {
        LIBS += -L../external/msvc2019/AdvancedDocking/lib -lqtadvanceddockingd
        LIBS += -L../../vcpkg/installed/x64-windows/debug/lib -lOpenImageIO
    }

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release

    COPIES += dlls


    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenColorIO/bin/OpenColorIO.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/vcruntime140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/vcruntime140_1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/msvcp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/concrt140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/vcomp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/msvcp140_1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/opengl32sw.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Svg.dll)

    CONFIG(release, debug|release) {
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/OpenImageIO.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Widgets.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Gui.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Core.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/AdvancedDocking/lib/qtadvanceddocking.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/boost_filesystem-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/boost_system-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/boost_thread-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/boost_chrono-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/boost_date_time-vc142-mt-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/zlib1.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/jpeg62.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/libpng16.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/IlmImf-2_5.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/Imath-2_5.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/Iex-2_5.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/Half-2_5.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/IlmThread-2_5.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/tiff.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/bin/lzma.dll)
    }

    CONFIG(debug, debug|release) {
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/OpenImageIO.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Widgetsd.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Guid.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/Qt/Qt5Cored.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/AdvancedDocking/lib/qtadvanceddockingd.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/boost_filesystem-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/boost_system-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/boost_thread-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/boost_chrono-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/boost_date_time-vc142-mt-gd-x64-1_74.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/zlibd1.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/jpeg62.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/libpng16d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/IlmImf-2_5_d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/Imath-2_5_d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/Iex-2_5_d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/Half-2_5_d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/IlmThread-2_5_d.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/tiffd.dll)
    dlls.files += $$files($$DESTDIR/../../../vcpkg/installed/x64-windows/debug/bin/lzmad.dll)
    }

    #dlls.files += $$files($$DESTDIR/../../external/msvc2019/Other/bin/*)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/expat/bin/libexpat.dll)
    dlls.path = $$DESTDIR

    CONFIG(release, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$DESTDIR/../../external/msvc2019/Qt/platforms/release/*)
    platforms.path = $$DESTDIR/platforms
    }

    CONFIG(debug, debug|release) {
    COPIES += platforms
    platforms.files += $$files($$DESTDIR/../../external/msvc2019/Qt/platforms/debug/*)
    platforms.path = $$DESTDIR/platforms
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

    COPIES += plugins
    plugins.files += $$files($$DESTDIR/../../plugins/ofx)
    plugins.path = $$DESTDIR/plugins

    INSTALLS += plugins
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss

