#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:52:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = app
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    server.cpp \
    main.cpp \
    clientthread.cpp

HEADERS += \
    server.h \
    clientthread.h

INCLUDEPATH += \
		../tools/ \
		../network/ \
		../module/

LIBS += \
		../tools/libtools.a \
		../network/libnetwork.a \
		../module/libmodule.a

LIBS += -ldl
