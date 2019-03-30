INCLUDEPATH += $$IDE_SOURCE_TREE/Component/TCPCommunication
DEPENDPATH += $$IDE_SOURCE_TREE/Component/TCPCommunication
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH

CONFIG(debug, debug|release): LIBS *= -lTCPCommunicationd
CONFIG(release, debug|release): LIBS *= -lTCPCommunication

message($$LIBS)
