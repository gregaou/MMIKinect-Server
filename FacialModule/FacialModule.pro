#-------------------------------------------------
#
# Project created by QtCreator 2013-05-02T10:11:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = FacialModule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

INCLUDEPATH += ../MMICore/

LIBS += ../libMMICore.a

LIBS += -ldl

CONFIG += link_pkgconfig

PKGCONFIG += opencv

DESTDIR = ../lib/

SOURCES += \
    FacialModule.cpp \
    FacialUtils.cpp

HEADERS += \
    FacialModule.h \
    FacialUtils.h
