TEMPLATE = app
TARGET = NanoMagView

QMAKE_CC = gcc
QMAKE_CXX = g++

CONFIG += c++11

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=  $$files(src/*.cpp)
HEADERS += $$files(include/*.hpp)
OBJECTS_DIR = obj

#INCLUDEPATH += $$PWD/VFRendering/libVFRendering.a
LIBS += $$PWD/VFRendering/libVFRendering.a

INCLUDEPATH += $$PWD/VFRendering/include
INCLUDEPATH += $$PWD/VFRendering/thirdparty/glm/include
#unix:!macx:LIBS += $$PWD/VFRendering/build/libVFRendering.so

clean.commands += rm obj/*;
unix:!macx:clean.commands += rm NanoMagView;
macx:clean.commands += rm -rf NanoMagView.app;

buildVF.commands = cd VFRendering; CC=$(CC) CXX=$(CXX) cmake ./; make
QMAKE_EXTRA_TARGETS += buildVF clean
# PRE_TARGETDEPS += buildVF
