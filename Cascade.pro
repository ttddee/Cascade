QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

#-------------------------------

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/colorbutton.cpp \
    src/colorbuttonentity.cpp \
    src/comboboxentity.cpp \
    src/connection.cpp \
    src/csimage.cpp \
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
    src/colorbutton.h \
    src/colorbuttonentity.h \
    src/comboboxentity.h \
    src/connection.h \
    src/csimage.h \
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux-g++ {
    LIBS += -L /usr/lib -lOpenImageIO
}

win32-msvc* {
    INCLUDEPATH += ../external/msvc2019/OpenImageIO/include
    INCLUDEPATH += ../../Documents/vcpkg/installed/x64-windows/include

    LIBS += -L../external/msvc2019/OpenImageIO/lib -lOpenImageIO
    LIBS += -LC:/VulkanSDK/1.2.148.0/Lib -lvulkan-1

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release

    COPIES += dlls
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/OpenImageIO.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/OpenImageIO_Util.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/vcruntime140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/vcruntime140_1.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/msvcp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/OpenImageIO/bin/concrt140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/vcomp140.dll)
    dlls.files += $$files($$DESTDIR/../../external/msvc2019/msvcp140_1.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/D3Dcompiler_47.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/libEGL.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/libEGLSV2.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/opengl32sw.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Svg.dll)
    dlls.files += $$files(C:/VulkanSDK/1.2.148.0/Lib/vulkan-1.lib)

    CONFIG(release, debug|release) {
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Widgets.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Gui.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Core.dll)
    }

    CONFIG(debug, debug|release) {
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Widgetsd.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Guid.dll)
    dlls.files += $$files(C:/Qt515/5.15.0/msvc2019_64/bin/Qt5Cored.dll)
    }

    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/IlmImf-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/IlmThread-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/Imath-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/Iex-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/Half-2_3.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/raw_r.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/jpeg62.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/libpng16.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/lzma.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/lcms2.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/jasper.dll)
    dlls.files += $$files($$DESTDIR/../../../Documents/vcpkg/installed/x64-windows/bin/zlib1.dll)
    dlls.files += $$files(C:/Program Files/Boost/boost_1_66_0/lib64-msvc-14.1/boost_filesystem-vc141-mt-x64-1_66.dll)
    dlls.files += $$files(C:/Program Files/Boost/boost_1_66_0/lib64-msvc-14.1/boost_system-vc141-mt-x64-1_66.dll)
    dlls.files += $$files(C:/Program Files/Boost/boost_1_66_0/lib64-msvc-14.1/boost_thread-vc141-mt-x64-1_66.dll)
    dlls.files += $$files(C:/Program Files/Boost/boost_1_66_0/lib64-msvc-14.1/boost_chrono-vc141-mt-x64-1_66.dll)
    dlls.files += $$files(C:/Program Files/Boost/boost_1_66_0/lib64-msvc-14.1/boost_date_time-vc141-mt-x64-1_66.dll)
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
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss
