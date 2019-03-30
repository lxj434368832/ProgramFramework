INCLUDEPATH += $$IDE_SOURCE_TREE/Controller/ControllerManage
DEPENDPATH += $$IDE_SOURCE_TREE/Controller/ControllerManage
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
CONFIG(debug, debug|release): LIBS *= -lControllerManaged
CONFIG(release, debug|release): LIBS *= -lControllerManage

message($$LIBS)
