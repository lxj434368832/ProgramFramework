
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
    FormStatistic.h \
    formAddData.h \
    FormLotteryNumber.h \
    FormLotteryNumber.h

SOURCES += \
    MMainWindow.cpp \
    FormStatistic.cpp \
    formAddData.cpp \
    FormLotteryNumber.cpp \
    FormLotteryNumber.cpp

FORMS += \
    MMainWindow.ui \
    FormStatistic.ui \
    formAddData.ui \
    FormLotteryNumber.ui \
    FormLotteryNumber.ui

