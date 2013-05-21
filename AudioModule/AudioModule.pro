#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T11:02:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = AudioModule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += audiomodule.cpp \
    audiosample.cpp

HEADERS += audiomodule.h \
    audiosample.h

INCLUDEPATH += ../MMICore/

LIBS += ../libMMICore.a

LIBS += -ldl

DESTDIR = ../lib/
