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
    src/connection.cpp \
    src/csimage.cpp \
    src/fileboxentity.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nodebase.cpp \
    src/nodegraph.cpp \
    src/nodegraphcontextmenu.cpp \
    src/nodeinput.cpp \
    src/nodeoutput.cpp \
    src/nodeproperties.cpp \
    src/propertiesview.cpp \
    src/rendermanager.cpp \
    src/uientity.cpp \
    src/viewerstatusbar.cpp \
    src/vulkanrenderer.cpp \
    src/vulkanview.cpp \
    src/vulkanwindow.cpp \
    src/windowmanager.cpp

HEADERS += \
    src/connection.h \
    src/csimage.h \
    src/fileboxentity.h \
    src/mainwindow.h \
    src/nodebase.h \
    src/nodedefinitions.h \
    src/nodegraph.h \
    src/nodegraphcontextmenu.h \
    src/nodeinput.h \
    src/nodeoutput.h \
    src/nodeproperties.h \
    src/propertiesview.h \
    src/rendermanager.h \
    src/uientity.h \
    src/viewerstatusbar.h \
    src/vulkanrenderer.h \
    src/vulkanview.h \
    src/vulkanwindow.h \
    src/windowmanager.h

FORMS += \
    src/fileboxentity.ui \
    src/mainwindow.ui \
    src/nodebase.ui \
    src/viewerstatusbar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux-g++ {

}

win32-msvc* {

}

RESOURCES += \
    resources.qrc

DISTFILES += \
    style/stylesheet.qss
