#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:52:19
#
#-------------------------------------------------

QT       -= core gui

TARGET = MMIKinect-Server
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

INCLUDEPATH += ../MMICore

LIBS += ../libMMICore.a

LIBS += -ldl

DESTDIR = ../
