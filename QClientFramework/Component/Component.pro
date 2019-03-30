
TEMPLATE = subdirs

exists($$PWD/DatabaseModule/DatabaseModule.pro):SUBDIRS += DatabaseModule

SUBDIRS +=  \
    TCPCommunication
#    MessageModule
