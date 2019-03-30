
include(../../configuration.pri)

QT       += core

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
CONFIG(debug, debug|release) {
    TARGET = ModelManaged
} else {
    TARGET = ModelManage
}

HEADERS += \
    IModelManage.h \
    ModelManage.h

SOURCES += \
    ModelManage.cpp
