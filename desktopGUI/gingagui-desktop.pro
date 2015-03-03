#QMAKE_LFLAGS += /INCREMENTAL:NO

TARGET = \
    gingagui

QT += \
    widgets

unix{
    target.path = /usr/bin
    INSTALLS += target

    data_desktop.path = /usr/share/applications/
    data_desktop.files = ginga.desktop
    data_icon.path = /usr/share/pixmaps/
    data_icon.files =  images/icons/icon-gingagui.ico
    INSTALLS += data_desktop data_icon
}

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

#QMAKE_LFLAGS = -static-libgcc -static-libstdc++


INCLUDEPATH += include\

SOURCES += \
    src/main.cpp \
    src/qnplmainwindow.cpp \
    src/qnplview.cpp \
    src/qnplsettings.cpp \
    src/qnplpreferencesdialog.cpp \
    src/qnplaboutdialog.cpp \
    src/qnplchannelsdialog.cpp \
    src/qnpliptvtunerdialog.cpp \
    src/qnplaplication.cpp \
    src/gingaproxy.cpp \
    src/developerview.cpp \
    src/util.cpp \
    src/debugview.cpp \
    src/debugobjectitem.cpp

HEADERS += \
    include/qnplmainwindow.h \
    include/qnplview.h \
    include/qnplsettings.h \
    include/qnplpreferencesdialog.h \
    include/qnplaboutdialog.h \
    include/qnplchannelsdialog.h \
    include/qnpliptvtunerdialog.h \
    include/qnplaplicationdialog.h \
    include/gingaproxy.h \
    include/developerview.h \
    include/util.h \
    include/debugview.h \
    include/debugobjectitem.h

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

OTHER_FILES += \
    ginga.desktop \
    application-x-ncl-NCL.xml \
    debian/rules \
    debian/copyright \
    debian/compat \
    debian/changelog \
    debian/files \
    debian/control \
    debian/postinst.ex
