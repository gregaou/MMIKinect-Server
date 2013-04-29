#-------------------------------------------------
#
# Project created by QtCreator 2013-04-29T19:46:13
#
#-------------------------------------------------

QT       -= core gui

TARGET = network
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    tcpsocketserver.cpp \
    tcpsocket.cpp \
    packet/packet.cpp

HEADERS += \
    tcpsocketserver.h \
    tcpsocket.h \
    networkexception.h \
    packet/score.h \
    packet/person.h \
    packet/packet.h \
    packet/networkvector.h \
    packet/inetworkmessage.h

INCLUDEPATH += ../tools/

LIBS += ../tools/libtools.a
