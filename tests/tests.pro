TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


LIBS += -L../build -lRS
INCLUDEPATH += ../include


SOURCES += \
    gftest.cpp \
    rstest.cpp \
    tests.cpp

HEADERS += \
    gftest.hpp \
    testsuite.hpp \
    rstest.hpp

