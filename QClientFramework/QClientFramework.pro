
TEMPLATE = subdirs
CONFIG   += ordered

exists($$PWD/Utility/Utility.pro):SUBDIRS += Utility

SUBDIRS +=  CommonFile \
    View \
    Model \
    Controller \
    MainClient \
    Component
