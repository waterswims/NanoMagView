TEMPLATE = app
TARGET = NanoMagView

CONFIG += c++11

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=  $$files(src/*.cpp)
HEADERS += $$files(include/*.hpp)
OBJECTS_DIR = obj

INCLUDEPATH += $$PWD/VFRendering/include
unix:!macx:LIBS += $$PWD/VFRendering/build/libVFRendering.so

buildVF.commands = cd VFRendering; make clean; make
QMAKE_EXTRA_TARGETS += buildVF
PRE_TARGETDEPS += buildVF
