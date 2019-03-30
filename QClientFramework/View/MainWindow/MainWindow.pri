INCLUDEPATH += $$IDE_SOURCE_TREE/ViewManage/MainWindow
DEPENDPATH += $$IDE_SOURCE_TREE/ViewManage/MainWindow
LIBS *= -L$$IDE_PLUGINS_DEPEND_PATH
CONFIG(debug, debug|release): LIBS *= -lMainWindowd
CONFIG(release, debug|release): LIBS *= -lMainWindow

message($$LIBS)
