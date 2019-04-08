
include(../../configuration.pri)

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += staticlib
DESTDIR  = $$IDE_APP_PATH
CONFIG(debug, debug|release) {
    TARGET = MainWindowd
} else {
    TARGET = MainWindow
}

HEADERS += \
   MMainWindow.h \
    FormStatistic.h

SOURCES += \
    MMainWindow.cpp \
    FormStatistic.cpp

FORMS += \
    MMainWindow.ui \
    FormStatistic.ui
