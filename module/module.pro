#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:49:25
#
#-------------------------------------------------

QT       -= core gui

TARGET = module
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    moduleserver.cpp \
    modulehandler.cpp \
    modulethread.cpp

HEADERS += \
    moduleserver.h \
    moduleexception.h \
    imodule.h \
    modulehandler.h \
    modulethread.h

INCLUDEPATH += \
		../tools/ \
		../network/

LIBS += \
		../tools/libtools.a \
		../network/libnetwork.a

LIBS += -ldl
