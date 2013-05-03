TARGET = \
    gingagui

VERSION = \
  1.0.6

DEFINES += \
    VERSION=\\\"$${VERSION}\\\"

INCLUDEPATH += \
    src \
    src/ui \
    src/util

SOURCES += \
    src/main.cpp \
    src/ui/qnplmainwindow.cpp \
    src/ui/qnplview.cpp \
    src/ui/qnplscene.cpp \
    src/ui/qnplsettings.cpp \
    src/ui/qnplpreferencesdialog.cpp \
    src/ui/qnplaboutdialog.cpp \
    src/util/qnplutil.cpp \
    src/ui/qnplchannelsdialog.cpp \
    src/ui/qnpliptvtunerdialog.cpp

HEADERS += \
    src/ui/qnplmainwindow.h \
    src/ui/qnplview.h \
    src/ui/qnplscene.h \
    src/ui/qnplsettings.h \
    src/ui/qnplpreferencesdialog.h \
    src/ui/qnplaboutdialog.h \
    src/util/qnplutil.h \
    src/ui/qnplchannelsdialog.h \
    src/ui/qnpliptvtunerdialog.h

RESOURCES += \
    application.qrc

RC_FILE = \
    application.rc

FORMS += \
    forms/qnplaboutform.ui \
    forms/qnplenvironmentform.ui \
    forms/qnplpreferencesform.ui \
    forms/qnplrunform.ui \
    forms/qnplchannelsform.ui \
    forms/qnpliptvtunerform.ui

TRANSLATIONS += \
    translations/en.ts \
    translations/es.ts \
    translations/pt_br.ts
