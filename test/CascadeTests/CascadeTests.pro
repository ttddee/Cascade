include(gtest_dependency.pri)

QT += widgets gui core testlib

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread

CONFIG += c++17

VERSION_MAJOR = 0
VERSION_MINOR = 2
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\
           "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}

# NOTE
# On Linux this expects the source files of gtest to be in /usr/src/gtest
# They might have been installed to /usr/src/googletest or something else
# In that case, the easiest is to create a symlink:
# sudo ln -s /usr/src/googletest /usr/src/gtest

HEADERS += \
        testheader.h \
        tst_node.h \
        tst_nodegraphdatamodel.h \
        tst_slider.h \
        ../../src/log.h \
        ../../src/ui/slider.h \
        ../../src/renderer/rendertask.h \
        ../../src/renderer/rendertaskread.h \
        $$files(../../src/nodegraph/*.h,          true) \
        $$files(../../src/nodegraph/nodes/*.h,    true) \
        $$files(../../src/properties/*.h,         true) \

SOURCES += \
        main.cpp \
        ../../src/log.cpp \
        ../../src/ui/slider.cpp \
        ../../src/renderer/rendertask.cpp \
        ../../src/renderer/rendertaskread.cpp \
        $$files(../../src/nodegraph/*.cpp,        true) \
        $$files(../../src/properties/*.cpp,       true) \

RESOURCES += \
    resources.qrc


