#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T18:51:22
#
#-------------------------------------------------
#QT       += phonon

QT       += core gui

#CONFIG  += static

TARGET = errorise-testing
TEMPLATE = app


SOURCES += main.cpp\
    myprocess.cpp \
    tracks.cpp \
    mediadata.cpp \
    mplayerprocess.cpp \
    global.cpp \
    colorutils.cpp \
    mplayerversion.cpp \
    translator.cpp \
    paths.cpp \
    preferences.cpp \
    mediasettings.cpp \
    amplayer.cpp \
    core.cpp \
    mediainfo.cpp \
    preferenceswindow.cpp \
    version.cpp \
    simpleplaylist.cpp \
    helper.cpp \
    simplegui.cpp \
    simpletoolbar.cpp \
    simplealbumlist.cpp \
    interfaces/queueinterface.cpp

HEADERS  += \
    myprocess.h \
    tracks.h \
    mediadata.h \
    config.h \
    mplayerprocess.h \
    global.h \
    preferences.h \
    colorutils.h \
    mplayerversion.h \
    translator.h \
    paths.h \
    mediasettings.h \
    amplayer.h \
    version.h \
    core.h \
    mediainfo.h \
    preferenceswindow.h \
    myslider.h \
    simpleplaylist.h \
    helper.h \
    simplegui.h \
    simpletoolbar.h \
    simplealbumlist.h \
    interfaces/queueinterface.h \
    interfaces/playerinterface.h

FORMS    += mainwindow.ui \
    preferenceswindow.ui
