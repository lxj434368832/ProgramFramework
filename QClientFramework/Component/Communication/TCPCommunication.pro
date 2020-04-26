
include(../../configuration.pri)

QT       += core

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH

CONFIG(debug, debug|release) {
    TARGET = TCPCommunicationd
} else {
    TARGET = TCPCommunication
}

HEADERS += \
    ServerConnect.h \
    ITCPCommunication.h \
    TCPCommunication.h

SOURCES += \
    ServerConnect.cpp \
    TCPCommunication.cpp


