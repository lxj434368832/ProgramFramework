LIBRARY_NAME        = ReadWriteExcel
LIBRARY_VERSION     = 1.0.1

LIBRARY_PATH = $$QT_3RDPARTYPATH/$$LIBRARY_NAME

exists( $${LIBRARY_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    LIBRARY_HEADERS   = $${LIBRARY_PATH}/include
    LIBRARY_LIBS      = $${LIBRARY_PATH}/lib

    INCLUDEPATH += $${LIBRARY_HEADERS}

    CONFIG(debug, debug|release): LIBS += -L$${LIBRARY_LIBS}/ -lReadWriteExceld
    CONFIG(release, debug|release): LIBS += -L$${LIBRARY_LIBS}/ -lReadWriteExcel

#    LIBRARY_install.files += $$LIBRARY_PATH/bin/*
#    LIBRARY_install.path = $${IDE_APP_PATH}/
#    INSTALLS += LIBRARY_install
}

!exists( $${LIBRARY_PATH} ) {
    message($${LIBRARY_PATH})
    warning("Cann't find ReadWriteExcel DIR =======================")
}
message($$LIBS)
#CONFIG += console
#DEFINES += QT_MESSAGELOGCONTEXT #¿ªÆôreleaseÏÂµÄlogcontext

#Release:DEFINES += QT_NO_WARNING_OUTPUT \
#QT_NO_DEBUG_OUTPUT
