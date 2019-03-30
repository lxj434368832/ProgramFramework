INCLUDEPATH += $$IDE_SOURCE_TREE/CommonFile
#DEPENDPATH += $$IDE_SOURCE_TREE/
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH

CONFIG(release, debug|release): LIBS *= -lCommonFile
CONFIG(debug, debug|release): LIBS *= -lCommonFiled

message($$LIBS)
