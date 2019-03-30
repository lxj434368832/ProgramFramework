
include(../configuration.pri)

QT       += core

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
CONFIG(debug, debug|release) {
    TARGET = CommonFiled
} else {
    TARGET = CommonFile
}


HEADERS += \
    CommonDefine.h \
    EnumDefine.h \
    TypeDefine.h

SOURCES += \
    TypeDefine.cpp
