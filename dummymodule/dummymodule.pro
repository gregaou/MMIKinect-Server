#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T20:01:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = dummymodule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += \
    test.cpp

HEADERS += \
    test.h

INCLUDEPATH += ../MMICore/

LIBS += ../libMMICore.a

LIBS += -ldl

DESTDIR = ../lib/
