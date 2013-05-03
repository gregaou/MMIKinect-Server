#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:39:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += \
    logger.cpp \
    threadprivate.cpp \
    thread.cpp

HEADERS += \
    typedef.h \
    singleton.h \
    logger.h \
    threadprivate.h \
    thread.h
