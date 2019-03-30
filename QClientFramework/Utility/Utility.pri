INCLUDEPATH += $$IDE_SOURCE_TREE/Utility
DEPENDPATH += $$IDE_SOURCE_TREE/Utility
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
CONFIG(debug, debug|release): LIBS *= -lUtilityd
CONFIG(release, debug|release): LIBS *= -lUtility

message($$LIBS)
