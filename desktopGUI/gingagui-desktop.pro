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
    src/gingaproxy.cpp \
    src/developerview.cpp \
    src/util.cpp \
    src/debugview.cpp \
    src/debugobjectitem.cpp \
    src/aboutdialog.cpp \
    src/channelsdialog.cpp \
    src/iptvtunerdialog.cpp \
    src/mainwindow.cpp \
    src/preferencesdialog.cpp \
    src/gingaview.cpp \


HEADERS += \
    include/gingaproxy.h \
    include/developerview.h \
    include/util.h \
    include/debugview.h \
    include/debugobjectitem.h \
    include/iptvtunerdialog.h \
    include/aboutdialog.h \
    include/channelsdialog.h \
    include/mainwindow.h \
    include/preferencesdialog.h \
    include/gingaview.h

RESOURCES += \
    application.qrc

RC_FILE = \
    application.rc

FORMS += \
    forms/aboutform.ui \
    forms/channelsform.ui \
    forms/environmentform.ui \
    forms/iptvtunerform.ui \
    forms/preferencesform.ui \
    forms/runform.ui

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
