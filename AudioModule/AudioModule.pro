#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T11:02:07
#
#-------------------------------------------------

QT       -= core gui

TARGET = AudioModule
TEMPLATE = lib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += audiomodule.cpp \
    audiosample.cpp

HEADERS += audiomodule.h \
    audiosample.h

INCLUDEPATH += ../MMICore/

LIBS += ../libMMICore.a

LIBS += -ldl

DESTDIR = ../lib/

LIBS += -L/home/theo/Dropbox/Cours/M1/S2/TER/LIA_RAL/lib/ -lliatools_Linux_i686

INCLUDEPATH += /home/theo/Dropbox/Cours/M1/S2/TER/LIA_RAL/LIA_SpkTools/include
DEPENDPATH += /home/theo/Dropbox/Cours/M1/S2/TER/LIA_RAL/LIA_SpkTools/include

PRE_TARGETDEPS += /home/theo/Dropbox/Cours/M1/S2/TER/LIA_RAL/lib/libliatools_Linux_i686.a

LIBS += -L/home/theo/Dropbox/Cours/M1/S2/TER/ALIZE/lib/ -lalize_Linux_i686

INCLUDEPATH += /home/theo/Dropbox/Cours/M1/S2/TER/ALIZE/include
DEPENDPATH += /home/theo/Dropbox/Cours/M1/S2/TER/ALIZE/include

PRE_TARGETDEPS += /home/theo/Dropbox/Cours/M1/S2/TER/ALIZE/lib/libalize_Linux_i686.a
