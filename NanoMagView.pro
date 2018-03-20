TEMPLATE = app
TARGET = NanoMagView

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13

QMAKE_CC = gcc
QMAKE_CXX = g++

CONFIG += c++11

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=  $$files(src/*.cpp)
HEADERS += $$files(include/*.hpp)
OBJECTS_DIR = obj
MOC_DIR = moc

LIBS += $$PWD/VFRendering/libVFRendering.a
LIBS += $$PWD/VFRendering/qhull-prefix/src/qhull-build/libqhullcpp.a
LIBS += $$PWD/VFRendering/qhull-prefix/src/qhull-build/libqhullstatic_r.a
LIBS += -ldl

INCLUDEPATH += $$PWD/VFRendering/include
INCLUDEPATH += $$PWD/VFRendering/thirdparty/glm/include

clean.commands += rm obj/*;
unix:!macx:clean.commands += rm NanoMagView;
macx:clean.commands += rm -rf NanoMagView.app;

buildVF.commands += cd VFRendering; rm -rf CMakeFiles; rm CMakeCache.txt;
buildVF.commands += rm qhull-prefix/src/qhull-build/CMakeCache.txt;
buildVF.commands += CC=$(CC) CXX=$(CXX) cmake ./; make
QMAKE_EXTRA_TARGETS += buildVF clean
# PRE_TARGETDEPS += buildVF
