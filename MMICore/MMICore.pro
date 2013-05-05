#-------------------------------------------------
#
# Project created by QtCreator 2013-05-05T11:02:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = MMICore
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    module/modulethread.cpp \
    module/moduleserver.cpp \
    module/modulehandler.cpp \
    network/tcpsocketserver.cpp \
    network/tcpsocket.cpp \
    network/packet/trainpacket.cpp \
    network/packet/scorepacket.cpp \
    network/packet/score.cpp \
    network/packet/person.cpp \
    network/packet/packet.cpp \
    network/packet/listpacket.cpp \
    tools/threadprivate.cpp \
    tools/thread.cpp \
    tools/logger.cpp

HEADERS += \
    module/modulethread.h \
    module/moduleserver.h \
    module/modulehandler.h \
    module/moduleexception.h \
    module/imodule.h \
    network/tcpsocketserver.h \
    network/tcpsocket.h \
    network/networkexception.h \
    network/packet/trainpacket.h \
    network/packet/scorepacket.h \
    network/packet/score.h \
    network/packet/person.h \
    network/packet/packet.h \
    network/packet/networkvector.h \
    network/packet/listpacket.h \
    network/packet/inetworkmessage.h \
    tools/typedef.h \
    tools/threadprivate.h \
    tools/thread.h \
    tools/singleton.h \
    tools/logger.h

QMAKE_LFLAGS += -Wl,--export-dynamic

LIBS += -ldl

DESTDIR = ../
