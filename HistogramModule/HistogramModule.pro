#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T11:26:58
#
#-------------------------------------------------

QT       -= core gui

TARGET = HistogramModule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += histogrammodule.cpp \
    histogramrecognizer.cpp \
    histogram.cpp

HEADERS += histogrammodule.h \
    histogramrecognizer.h \
    histogram.h

INCLUDEPATH += ../MMICore/

LIBS += ../libMMICore.a

LIBS += -ldl

INCLUDEPATH += /usr/include/opencv \
							 /usr/include/opencv2 \
							 /usr/include/opencv2/core

LIBS += -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_imgproc


DESTDIR = ../lib/
