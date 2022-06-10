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
        tst_slider.h \
        #$$files(../../src/*.h,                    true) \
        #$$files(../../src/codeeditor/*.h,         true) \
#        $$files(../../src/docking/*.h,            true) \
#        $$files(../../src/nodegraph/*.h,          true) \
#        $$files(../../src/nodegraph/nodes/*.h,    true) \
#        $$files(../../src/properties/*.h,         true) \
#        $$files(../../src/renderer/*.h,           true) \
#        $$files(../../src/shadercompiler/*.h,     true) \
#        $$files(../../src/log.h,                  true) \
        #$$files(../../src/ui/*.h,                 true) \
        #$$files(../../src/uientities/*.h,         true) \

SOURCES += \
        main.cpp \
        #$$files(../../src/*.cpp,                  true) \
        #$$files(../../src/codeeditor/*.cpp,       true) \
#        $$files(../../src/docking/*.cpp,          true) \
#        $$files(../../src/nodegraph/*.cpp,        true) \
#        $$files(../../src/nodegraph/nodes/*.cpp,  true) \
#        $$files(../../src/properties/*.cpp,       true) \
#        $$files(../../src/renderer/*.cpp,         true) \
#        $$files(../../src/shadercompiler/*.cpp,   true) \
        #$$files(../../src/ui/*.cpp,               true) \
        #$$files(../../src/uientities/*.cpp,       true) \

SOURCES -= \
        #../../src/main.cpp

FORMS += \
        #$$files(../../src/*.ui,                   true) \
        #$$files(../../src/uientities\*.ui,        true) \

INCLUDEPATH += \
        ../../src \


SRC_DIR = ../../src

# Copy the whole project because qmake creates dependencies in
# the compiled ui files that won't resolve otherwise
#COPIES += srcfiles
#srcfiles.files += $$files(../../src)
#srcfiles.path = $$OUT_PWD

#COPIES += srcfiles
#srcfiles.files += $$files(../../src/slidernoclick.h)
#srcfiles.files += $$files(../../src/colorbutton.h)
#srcfiles.path = $$OUT_PWD/src


