#-------------------------------------------------
#
# Project created by QtCreator 2013-05-02T10:11:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = FacialModule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

INCLUDEPATH += \
                ../tools/ \
                ../network/ \
                ../module/

LIBS += \
                ../tools/libtools.a \
                ../network/libnetwork.a \
                ../module/libmodule.a

LIBS += -ldl

INCLUDEPATH += /usr/include/opencv \
               /usr/include/opencv2 \
               /usr/include/opencv2/core

LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_imgproc

DESTDIR = ../lib/

SOURCES += \
    FacialModule.cpp

HEADERS += \
    FacialModule.h
