
TEMPLATE = subdirs

#exists($$PWD/DatabaseModule/DatabaseModule.pro):SUBDIRS += DatabaseModule
exists($$PWD/Utility/Utility.pro):SUBDIRS += Utility

SUBDIRS +=  \
    TCPCommunication
#    MessageModule
