MLOG_NAME        = MLog
MLOG_VERSION     = 1.0.1

MLOG_PATH = $$QT_3RDPARTYPATH/$$MLOG_NAME

exists( $${MLOG_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    MLOG_HEADERS   = $${MLOG_PATH}/include
    MLOG_LIBS      = $${MLOG_PATH}/lib

    INCLUDEPATH += $${MLOG_HEADERS}

    CONFIG(debug, debug|release): LIBS += -L$${MLOG_LIBS}/ -lMLogd
    CONFIG(release, debug|release): LIBS += -L$${MLOG_LIBS}/ -lMLog

#    MLOG_install.files += $$MLOG_PATH/bin/*
#    MLOG_install.path = $${IDE_APP_PATH}/
#    INSTALLS += MLOG_install
}

!exists( $${MLOG_PATH} ) {
    message($${MLOG_PATH})
    warning("Cann't find MLog DIR =======================")
}
#CONFIG += console
#DEFINES += QT_MESSAGELOGCONTEXT #¿ªÆôreleaseÏÂµÄlogcontext

#Release:DEFINES += QT_NO_WARNING_OUTPUT \
#QT_NO_DEBUG_OUTPUT
