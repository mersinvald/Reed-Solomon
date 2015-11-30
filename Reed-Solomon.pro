TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS += -O0

SOURCES += main.cpp \
    gf.cpp \
    gf_test.cpp \
    rs.cpp \
    rs_test.cpp \
    polyhandler.cpp \
    polyhandler_test.cpp

HEADERS += \
    gf.hpp \
    rs.hpp \
    polyhandler.h

