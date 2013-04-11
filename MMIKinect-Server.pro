#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T20:06:42
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = MMIKinect-Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    message.cpp \
    clientthread.cpp

HEADERS += \
    server.h \
    message.h \
    clientthread.h
