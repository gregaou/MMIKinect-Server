#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T11:02:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = AudioModule
TEMPLATE = lib

DEFINES += AUDIOMODULE_LIBRARY

SOURCES += audiomodule.cpp

HEADERS += audiomodule.h

INCLUDEPATH += \
		../tools/ \
		../network/ \
		../module/

LIBS += \
		../tools/libtools.a \
		../network/libnetwork.a \
		../module/libmodule.a

LIBS += -ldl

DESTDIR = ../lib/
