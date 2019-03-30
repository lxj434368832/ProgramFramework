INCLUDEPATH += $$IDE_SOURCE_TREE/Compoment/MessageModule
DEPENDPATH += $$IDE_SOURCE_TREE/Compoment/MessageModule
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH

CONFIG(debug, debug|release): LIBS *= -lMessageModuled
CONFIG(release, debug|release): LIBS *= -lMessageModule
#LIBS *= -l$$qtLibraryTarget(MessageModule)

message($$LIBS)
