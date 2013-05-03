#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:46:13
#
#-------------------------------------------------

QT       -= core gui

TARGET = network
TEMPLATE = lib
CONFIG += staticlib

QMAKE_LFLAGS += -Wl,--export-dynamic

SOURCES += \
    tcpsocketserver.cpp \
    tcpsocket.cpp \
    packet/packet.cpp \
    packet/trainpacket.cpp \
    packet/scorepacket.cpp \
    packet/score.cpp \
    packet/person.cpp \
    packet/listpacket.cpp

HEADERS += \
    tcpsocketserver.h \
    tcpsocket.h \
    networkexception.h \
    packet/score.h \
    packet/person.h \
    packet/packet.h \
    packet/networkvector.h \
    packet/inetworkmessage.h \
    packet/trainpacket.h \
    packet/scorepacket.h \
    packet/listpacket.h

INCLUDEPATH += ../tools/

LIBS += ../tools/libtools.a
