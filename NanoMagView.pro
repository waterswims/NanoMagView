TEMPLATE = app
TARGET = NanoMagView

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=  $$files(src/*.cpp)

HEADERS += $$files(include/*.hpp)

OBJECTS_DIR = obj

CONFIG += c++11

INCLUDEPATH += $$PWD/VFRendering/include

LIBS += $$PWD/VFRendering/build/libVFRendering.so
