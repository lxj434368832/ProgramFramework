INCLUDEPATH += $$IDE_SOURCE_TREE/ViewManage/ViewManage
DEPENDPATH += $$IDE_SOURCE_TREE/ViewManage/ViewManage
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
CONFIG(debug, debug|release): LIBS *= -lViewManaged
CONFIG(release, debug|release): LIBS *= -lViewManage

message($$LIBS)
