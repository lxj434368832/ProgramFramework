MCONFIGMANAGE_NAME        = MConfigManage
MCONFIGMANAGE_VERSION     = 1.0.1

MCONFIGMANAGE_PATH = $$QT_3RDPARTYPATH/$$MCONFIGMANAGE_NAME

exists( $${MCONFIGMANAGE_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    MCONFIGMANAGE_HEADERS   = $${MCONFIGMANAGE_PATH}/include
    MCONFIGMANAGE_LIBS      = $${MCONFIGMANAGE_PATH}/lib

    INCLUDEPATH += $${MCONFIGMANAGE_HEADERS}

    CONFIG(debug, debug|release): LIBS += -L$${MCONFIGMANAGE_LIBS}/ -lMConfigManaged
    CONFIG(release, debug|release): LIBS += -L$${MCONFIGMANAGE_LIBS}/ -lMConfigManage

#    MCONFIGMANAGE_install.files += $$MCONFIGMANAGE_PATH/bin/*
#    MCONFIGMANAGE_install.path = $${IDE_APP_PATH}/
#    INSTALLS += MCONFIGMANAGE_install
}

!exists( $${MCONFIGMANAGE_PATH} ) {
    message($${MCONFIGMANAGE_PATH})
    warning("Cann't find MCONFIGMANAGE DIR =======================")
}
message($$LIBS)
