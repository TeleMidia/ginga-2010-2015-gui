TARGET = \
    gingagui

QT += \
    widgets

#VERSION = \
#  1.0.6

#*-msvc* {
#	DEFINES += \
#		VERSION=\"$${VERSION}\"
#}
#else {
#	DEFINES += \
#		VERSION=\\\"$${VERSION}\\\"
#}


INCLUDEPATH += include\


SOURCES += \
    src/main.cpp \
    src/qnplmainwindow.cpp \
    src/qnplview.cpp \
    src/qnplscene.cpp \
    src/qnplsettings.cpp \
    src/qnplpreferencesdialog.cpp \
    src/qnplaboutdialog.cpp \
    src/qnplutil.cpp \
    src/qnplchannelsdialog.cpp \
    src/qnpliptvtunerdialog.cpp \
    src/qnplaplication.cpp

HEADERS += \
    include/qnplmainwindow.h \
    include/qnplview.h \
    include/qnplscene.h \
    include/qnplsettings.h \
    include/qnplpreferencesdialog.h \
    include/qnplaboutdialog.h \
    include/qnplutil.h \
    include/qnplchannelsdialog.h \
    include/qnpliptvtunerdialog.h \
    include/qnplaplicationdialog.h

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
    forms/qnpliptvtunerform.ui \
    forms/qnplaplicationform.ui

TRANSLATIONS += \
    translations/en.ts \
    translations/es.ts \
    translations/pt_br.ts
