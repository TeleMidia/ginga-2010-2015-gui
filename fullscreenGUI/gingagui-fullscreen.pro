#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T17:42:11
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gingagui
TEMPLATE = app

target.path = /usr/local/bin

xmls.path = /usr/local/lib/ginga/gui/files/xml
xmls.files += files/xml/*

INSTALLS += target xmls

INCLUDEPATH += include\

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/page.cpp \
    src/pagexmlparser.cpp \
    src/menuitem.cpp \
    src/gingaproxy.cpp \
    src/gingapage.cpp \
    src/infobar.cpp \
    src/useraccountpage.cpp \
    src/defaultrichmenuitem.cpp \
    src/comborichmenuitem.cpp \
    src/userpreferences.cpp \
    src/ippage.cpp

HEADERS  += include/mainwindow.h \
    include/page.h \
    include/util.h \
    include/pagexmlparser.h \
    include/menuitem.h \
    include/gingaproxy.h \
    include/gingapage.h \
    include/infobar.h \
    include/useraccountpage.h \
    include/richmenuitem.h \
    include/defaultrichmenuitem.h \
    include/comborichmenuitem.h \
    include/userpreferences.h \
    include/ippage.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
