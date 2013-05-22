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

INCLUDEPATH += /usr/local/include/opencv \
               /usr/local/include/opencv2 \
               /usr/local/include/opencv2/core

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_video -lopencv_features2d -lopencv_ml -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_legacy
# LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_imgproc

DESTDIR = ../lib/

SOURCES += \
    FacialModule.cpp

HEADERS += \
    FacialModule.h
