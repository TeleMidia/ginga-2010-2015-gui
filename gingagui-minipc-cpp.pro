#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T17:42:11
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gingagui-minipc-cpp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    page.cpp \
    pagexmlparser.cpp \
    menuitem.cpp \
    gingaproxy.cpp \
    gingapage.cpp \
    infobar.cpp \
    useraccountpage.cpp \
    defaultrichmenuitem.cpp \
    comborichmenuitem.cpp \
    userpreferences.cpp \
    ippage.cpp

HEADERS  += mainwindow.h \
    page.h \
    util.h \
    pagexmlparser.h \
    menuitem.h \
    gingaproxy.h \
    gingapage.h \
    infobar.h \
    useraccountpage.h \
    richmenuitem.h \
    defaultrichmenuitem.h \
    comborichmenuitem.h \
    userpreferences.h \
    ippage.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
