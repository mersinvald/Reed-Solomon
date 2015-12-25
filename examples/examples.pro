TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS = -Wall -O0

LIBS += -L../build -lRS
INCLUDEPATH += ../include

SOURCES += \
    example1.cpp

HEADERS += \
    ../include/gf.hpp \
    ../include/poly.hpp \
    ../include/rs.hpp

