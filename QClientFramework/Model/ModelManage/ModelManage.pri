INCLUDEPATH += $$IDE_SOURCE_TREE/Model/ModelManage/include
DEPENDPATH += $$IDE_SOURCE_TREE/Model/ModelManage/include
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
CONFIG(debug, debug|release): LIBS *= -lModelManaged
CONFIG(release, debug|release): LIBS *= -lModelManage

message($$LIBS)
