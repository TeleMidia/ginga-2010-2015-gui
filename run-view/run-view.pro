#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T17:20:24
#
#-------------------------------------------------
TARGET = RunView

include (plugins-common.pri)

INCLUDEPATH += . \
                ..\desktopGUI\include \
            include


macx:LIBS +=    -L$$quote(/Library/Application Support/Composer/Extensions) \
                -lNCLLanguageProfile

win32:LIBS +=   -lNCLLanguageProfile

SOURCES += \
    src/runviewfactory.cpp \
    src/runviewplugin.cpp \
    ../desktopGUI/src/qnplview.cpp \
    ../desktopGUI/src/qnplscene.cpp \
    ../desktopGUI/src/util.cpp \
    ../desktopGUI/src/gingaproxy.cpp \
    src/configdialog.cpp

HEADERS  += \
    include/runviewfactory.h \
    include/runviewplugin.h \
    ../desktopGUI/include/qnplview.h \
    ../desktopGUI/include/qnplscene.h \
    ../desktopGUI/include/util.h \
    ../desktopGUI/include/gingaproxy.h \
    include/configdialog.h

RESOURCES += \
    resources.qrc

FORMS += \
    ../desktopGUI/forms/qnplrunform.ui
