
include(../../configuration.pri)

QT       += core

TEMPLATE = lib
CONFIG += staticlib
#TARGET = $$qtLibraryTarget(LogicManage)
DESTDIR  = $$IDE_APP_PATH
CONFIG(debug, debug|release) {
    TARGET = ControllerManaged
} else {
    TARGET = ControllerManage
}

HEADERS += \
    ControllerManage.h \
    IControllerManage.h \
    IControllerManage.h

SOURCES += \
    ControllerManage.cpp

