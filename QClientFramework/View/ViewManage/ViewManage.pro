
include(../../configuration.pri)

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
CONFIG(debug, debug|release) {
    TARGET = ViewManaged
} else {
    TARGET = ViewManage
}

HEADERS += \
    ViewMediator.h \
    ViewColleague.h \
    ViewManage.h \
    IViewManage.h

SOURCES += \
    ViewMediator.cpp \
    ViewColleague.cpp \
    ViewManage.cpp


