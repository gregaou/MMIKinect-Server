TEMPLATE = app
CONFIG += console qt
CONFIG -= app_bundle

SOURCES += main.cpp \
    server.cpp \
    tcpsocket.cpp \
    tcpsocketserver.cpp \
    clientthread.cpp \
    packet.cpp \
    facetracking.cpp

HEADERS += \
    server.h \
    tcpsocket.h \
    networkexception.h \
    tcpsocketserver.h \
    clientthread.h \
    packet.h \
    typedef.h \
    facetracking.h \
    singleton.h \
    point2D.h \
    rect.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

RESOURCES +=

