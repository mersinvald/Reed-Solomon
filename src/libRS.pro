TARGET = RS
TEMPLATE = lib
DEFINES += RS_LIBRARY
DEFINES += DEBUG

QT       -= core gui
CONFIG += staticlib
CONFIG += c++11
CONFIG -= qt

QMAKE_CXX_FLAGS_DEBUG += -O0
QMAKE_CXX_FLAGS_RELEASE += -O3

DESTDIR = ../build/
OBJECTS_DIR = ../build/obj

INCLUDEPATH += ../include

SOURCES += \
    gf.cpp 

HEADERS += \
    ../include/gf.hpp \
    ../include/rs.hpp \
    ../include/poly.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
