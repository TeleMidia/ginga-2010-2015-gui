TARGET = \
    gingagui

VERSION = \
    1.0.0

#CONFIG += release
#CONFIG -= debug
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING

RC_FILE = \
    application.rc

SOURCES += \
    src/main.cpp \
    src/ui/qnplmainwindow.cpp \
    src/ui/qnplview.cpp \
    src/ui/qnplscene.cpp \
    src/ui/qnplsettings.cpp \
    src/ui/qnplpreferencesdialog.cpp \
    src/ui/qnplaboutdialog.cpp

HEADERS += \
    src/ui/qnplmainwindow.h \
    src/ui/qnplview.h \
    src/ui/qnplscene.h \
    src/ui/qnplsettings.h \
    src/ui/qnplpreferencesdialog.h \
    src/ui/qnplaboutdialog.h

RESOURCES += \
    application.qrc

FORMS += \
    forms/QnplPreferencesForm.ui \
    forms/QnplGeneralForm.ui \
    forms/QnplRunForm.ui \
    forms/QnplAboutForm.ui

TRANSLATIONS += \
    translations/en.ts \
    translations/es.ts \
    translations/pt_br.ts



