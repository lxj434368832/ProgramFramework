
PROTOBUF_NAME        = Protobuf
PROTOBUF_VERSION     = 3.0.6

#PROTOBUF_PATH = $${PWD}/$$PROTOBUF_NAME/$$PROTOBUF_VERSION

PROTOBUF_PATH = $${PWD}


exists( $${PROTOBUF_PATH} ) {
    ######################################################################
    # Include library
    ######################################################################
    PROTOBUF_HEADERS   = $${PROTOBUF_PATH}/include
    PROTOBUF_LIBS      = $${PROTOBUF_PATH}/lib

    INCLUDEPATH += $${PROTOBUF_HEADERS}

    CONFIG(debug, debug|release) {
        LIBS += -L$${PROTOBUF_LIBS}/ -llibprotobufd \
            -L$${PROTOBUF_LIBS}/ -llibprotocd
    }
    CONFIG(release, debug|release) {
        LIBS += -L$${PROTOBUF_LIBS}/ -llibprotobuf \
            -L$${PROTOBUF_LIBS}/ -llibprotoc
    }

    PROTOBUF_install.files += $$PROTOBUF_PATH/bin/*
    PROTOBUF_install.path = $${IDE_APP_PATH}/
    INSTALLS += PROTOBUF_install
}

!exists( $${PROTOBUF_PATH} ) {
    warning("Cann't find PROTOBUF DIR =======================")
}
