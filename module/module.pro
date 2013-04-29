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
    moduleloader.cpp

HEADERS += \
    moduleserver.h \
    moduleloader.h \
    moduleexception.h \
    imodule.h

INCLUDEPATH += \
		../tools/ \
		../network/

LIBS += \
		../tools/libtools.a \
		../network/libnetwork.a

LIBS += -ldl
