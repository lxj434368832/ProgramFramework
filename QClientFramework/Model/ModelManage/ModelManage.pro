
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
    include\IModelManage.h \
    src\ModelManage.h \
    src\MainModel.h

SOURCES += \
    src\ModelManage.cpp \
    src\MainModel.cpp
