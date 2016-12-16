TARGET = RunView

include (../../plugins-common.pri)

INCLUDEPATH += . \
  ../desktop-gui/include \
  include


macx:LIBS += \
  -L$$quote(/Library/Application Support/Composer/Plugins) \
  -lNCLLanguageProfile

win32:LIBS += \
  -lNCLLanguageProfile

SOURCES += \
  src/runviewfactory.cpp \
  src/runviewplugin.cpp \
  ../desktop-gui/src/gingaview.cpp \
  ../desktop-gui/src/util.cpp \
  ../desktop-gui/src/gingaproxy.cpp \
  src/configdialog.cpp

HEADERS  += \
  include/runviewfactory.h \
  include/runviewplugin.h \
  ../desktop-gui/include/gingaview.h \
  ../desktop-gui/include/util.h \
  ../desktop-gui/include/gingaproxy.h \
  include/configdialog.h

RESOURCES += \
  resources.qrc

FORMS += \
  ../desktop-gui/forms/runform.ui
