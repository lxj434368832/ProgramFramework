MFRAMELESSWIDGET_NAME        = MFramelessWidget
MFRAMELESSWIDGET_VERSION     = 1.0.1

MFRAMELESSWIDGET_PATH = $$QT_3RDPARTYPATH/$$MFRAMELESSWIDGET_NAME

exists( $${MFRAMELESSWIDGET_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    MFRAMELESSWIDGET_HEADERS   = $${MFRAMELESSWIDGET_PATH}/include
    MFRAMELESSWIDGET_LIBS      = $${MFRAMELESSWIDGET_PATH}/lib

    INCLUDEPATH += $${MFRAMELESSWIDGET_HEADERS}

#    unix {
#        Debug:LIBS += -L$${MFRAMELESSWIDGET_LIBS}/Debug/ -lMFramelessWidget
#        Release:LIBS += -L$${MFRAMELESSWIDGET_LIBS}/Release/ -lMFramelessWidget
#    }
win32 {
    Debug:LIBS += -L$${MFRAMELESSWIDGET_LIBS}/ -lMFramelessWidgetd
    Release:LIBS += -L$${MFRAMELESSWIDGET_LIBS}/ -lMFramelessWidget
}

#    MFRAMELESSWIDGET_install.files += $$MFRAMELESSWIDGET_PATH/bin/*
#    MFRAMELESSWIDGET_install.path = $${IDE_APP_PATH}/
#    INSTALLS += MFRAMELESSWIDGET_install
}

!exists( $${MFRAMELESSWIDGET_PATH} ) {
    message($${MFRAMELESSWIDGET_PATH})
    warning("Cann't find MFRAMELESSWIDGET DIR =======================")
}
