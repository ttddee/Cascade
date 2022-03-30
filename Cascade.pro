QT       += core gui widgets

CONFIG += c++17

QMAKE_CXXFLAGS_RELEASE -= -O2

#------------------------------- Versioning

VERSION_MAJOR = 0
VERSION_MINOR = 2
VERSION_BUILD = 0

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00

#-------------------------------

RC_ICONS = cascade.ico

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/aboutdialog.cpp \
    src/codeeditor/QCXXHighlighter.cpp \
    src/codeeditor/QCodeEditor.cpp \
    src/codeeditor/QFramedTextAttribute.cpp \
    src/codeeditor/QGLSLCompleter.cpp \
    src/codeeditor/QGLSLHighlighter.cpp \
    src/codeeditor/QJSONHighlighter.cpp \
    src/codeeditor/QLanguage.cpp \
    src/codeeditor/QLineNumberArea.cpp \
    src/codeeditor/QLuaCompleter.cpp \
    src/codeeditor/QLuaHighlighter.cpp \
    src/codeeditor/QPythonCompleter.cpp \
    src/codeeditor/QPythonHighlighter.cpp \
    src/codeeditor/QStyleSyntaxHighlighter.cpp \
    src/codeeditor/QSyntaxStyle.cpp \
    src/codeeditor/QXMLHighlighter.cpp \
    src/colorbutton.cpp \
    src/connection.cpp \
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
    src/isfmanager.cpp \
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
    src/preferencesdialog.cpp \
    src/preferencesmanager.cpp \
    src/projectmanager.cpp \
    src/propertiesheading.cpp \
    src/propertiesview.cpp \
    src/renderer/cscommandbuffer.cpp \
    src/renderer/csimage.cpp \
    src/renderer/cssettingsbuffer.cpp \
    src/renderer/vulkanrenderer.cpp \
    src/rendermanager.cpp \
    src/shadercompiler/SpvShaderCompiler.cpp \
    src/slidernoclick.cpp \
    src/uientities/channelselectentity.cpp \
    src/uientities/checkboxentity.cpp \
    src/uientities/codeeditorentity.cpp \
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
    src/aboutdialog.h \
    src/benchmark.h \
    src/codeeditor/QCXXHighlighter.hpp \
    src/codeeditor/QCodeEditor.hpp \
    src/codeeditor/QFramedTextAttribute.hpp \
    src/codeeditor/QGLSLCompleter.hpp \
    src/codeeditor/QGLSLHighlighter.hpp \
    src/codeeditor/QHighlightBlockRule.hpp \
    src/codeeditor/QHighlightRule.hpp \
    src/codeeditor/QJSONHighlighter.hpp \
    src/codeeditor/QLanguage.hpp \
    src/codeeditor/QLineNumberArea.hpp \
    src/codeeditor/QLuaCompleter.hpp \
    src/codeeditor/QLuaHighlighter.hpp \
    src/codeeditor/QPythonCompleter.hpp \
    src/codeeditor/QPythonHighlighter.hpp \
    src/codeeditor/QStyleSyntaxHighlighter.hpp \
    src/codeeditor/QSyntaxStyle.hpp \
    src/codeeditor/QXMLHighlighter.hpp \
    src/colorbutton.h \
    src/connection.h \
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
    src/global.h \
    src/isfmanager.h \
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
    src/popupmessages.h \
    src/preferencesdialog.h \
    src/preferencesmanager.h \
    src/projectmanager.h \
    src/propertiesheading.h \
    src/propertiesview.h \
    src/renderer/cscommandbuffer.h \
    src/renderer/csimage.h \
    src/renderer/cssettingsbuffer.h \
    src/renderer/renderconfig.h \
    src/renderer/renderutility.h \
    src/renderer/vulkanhppinclude.h \
    src/renderer/vulkanrenderer.h \
    src/rendermanager.h \
    src/shadercompiler/DirStackFileIncluder.h \
    src/shadercompiler/SpvShaderCompiler.h \
    src/slidernoclick.h \
    src/uicolors.h \
    src/uientities/channelselectentity.h \
    src/uientities/checkboxentity.h \
    src/uientities/codeeditorentity.h \
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
    INCLUDEPATH += $$PWD/external/OpenColorIO/install/include
    INCLUDEPATH += $$PWD/external/glslang/include

    LIBS += -L/usr/local/lib -lOpenImageIO
    LIBS += -L/usr/local/lib -lOpenImageIO_Util
    LIBS += -L$$PWD/external/OpenColorIO/install -lOpenColorIO
    LIBS += -L/usr/lib/x86_64-linux-gnu -ldl
    LIBS += -L/usr/lib/x86_64-linux-gnu -ltbb
    # The link order of the following libs is important
    LIBS += -L$$PWD/external/glslang/lib -lSPIRV
    LIBS += -L$$PWD/external/glslang/lib -lSPIRV-Tools-opt
    LIBS += -L$$PWD/external/glslang/lib -lSPIRV-Tools
    LIBS += -L$$PWD/external/glslang/lib -lMachineIndependent
    LIBS += -L$$PWD/external/glslang/lib -lglslang
    LIBS += -L$$PWD/external/glslang/lib -lglslang-default-resource-limits
    LIBS += -L$$PWD/external/glslang/lib -lOSDependent
    LIBS += -L$$PWD/external/glslang/lib -lOGLCompiler
    LIBS += -L$$PWD/external/glslang/lib -lGenericCodeGen

    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug
    CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
}

win32-msvc* {
    DEPENDENCY_ROOT = vcpkg_installed/x64-windows
    LIB_ROOT = ../vcpkg_installed/x64-windows
    QT_ROOT = C:/Qt515/5.15.0/msvc2019_64

    INCLUDEPATH += $$DEPENDENCY_ROOT/include
    INCLUDEPATH += C:/VulkanSDK/1.2.198.1/include

    COPIES += dlls
    COPIES += platforms

    CONFIG(debug, debug|release) {
        DESTDIR = $$OUT_PWD/debug

        # Debug Libs
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenImageIO_d
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenImageIO_Util_d
        LIBS += -L$$LIB_ROOT/debug/lib -lOpenColorIO
        LIBS += -L$$LIB_ROOT/debug/lib -ltbb_debug
        LIBS += -L$$LIB_ROOT/debug/lib -lglslangd
        LIBS += -L$$LIB_ROOT/debug/lib -lglslang-default-resource-limitsd
        LIBS += -L$$LIB_ROOT/debug/lib -lGenericCodeGend
        LIBS += -L$$LIB_ROOT/debug/lib -lMachineIndependentd
        LIBS += -L$$LIB_ROOT/debug/lib -lOGLCompilerd
        LIBS += -L$$LIB_ROOT/debug/lib -lOSDependentd
        LIBS += -L$$LIB_ROOT/debug/lib -lSPIRVd
        LIBS += -L$$LIB_ROOT/debug/lib -lSPVRemapperd

        # Debug DLLs
        dlls.files += $$files($$QT_ROOT/bin/Qt5Svgd.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Widgetsd.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Guid.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Cored.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/tbb_debug.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenImageIO_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenImageIO_Util_d.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/OpenColorIO_2_1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_filesystem-vc142-mt-gd-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_system-vc142-mt-gd-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_thread-vc142-mt-gd-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_chrono-vc142-mt-gd-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/boost_date_time-vc142-mt-gd-x64-1_78.dll)
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
        dlls.files += $$files($$DEPENDENCY_ROOT/debug/bin/liblzma.dll)

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
        LIBS += -L$$LIB_ROOT/lib -lglslang
        LIBS += -L$$LIB_ROOT/lib -lglslang-default-resource-limits
        LIBS += -L$$LIB_ROOT/lib -lGenericCodeGen
        LIBS += -L$$LIB_ROOT/lib -lMachineIndependent
        LIBS += -L$$LIB_ROOT/lib -lOGLCompiler
        LIBS += -L$$LIB_ROOT/lib -lOSDependent
        LIBS += -L$$LIB_ROOT/lib -lSPIRV
        LIBS += -L$$LIB_ROOT/lib -lSPVRemapper

        # Release DLLs
        dlls.files += $$files($$QT_ROOT/bin/Qt5Svg.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Widgets.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Gui.dll)
        dlls.files += $$files($$QT_ROOT/bin/Qt5Core.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/tbb.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenImageIO.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenImageIO_Util.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/OpenColorIO_2_1.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_filesystem-vc142-mt-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_system-vc142-mt-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_thread-vc142-mt-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_chrono-vc142-mt-x64-1_78.dll)
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/boost_date_time-vc142-mt-x64-1_78.dll)
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
        dlls.files += $$files($$DEPENDENCY_ROOT/bin/liblzma.dll)

        platforms.files += $$files($$QT_ROOT/plugins/platforms/qdirect2d.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qminimal.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qoffscreen.dll)
        platforms.files += $$files($$QT_ROOT/plugins/platforms/qwindows.dll)
    }

    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/concrt140.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/msvcp140.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/msvcp140_1.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/msvcp140_2.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/vcruntime140.dll)
    dlls.files += $$files($$DEPENDENCY_ROOT/../../windows/vcruntime140_1.dll)

    dlls.path = $$DESTDIR
    platforms.path = $$DESTDIR/platforms

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

COPIES += isf
isf.files += $$files($$PWD/shaders/isf/*.fs)
isf.path = $$DESTDIR/isf

RESOURCES += \
    ads.qrc \
    resources.qrc \
    src/codeeditor/qcodeeditor_resources.qrc

DISTFILES += \
    style/stylesheet.qss

