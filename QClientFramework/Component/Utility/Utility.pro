
include(../../configuration.pri)

QT       += core

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH

CONFIG(debug, debug|release) {
    TARGET = Utilityd
} else {
    TARGET = Utility
}

HEADERS += \
    MUtilityUI.h

SOURCES += \
    MUtilityUI.cpp
