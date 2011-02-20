#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T18:51:22
#
#-------------------------------------------------
QT       += phonon

QT       += core gui

#CONFIG  += static

TARGET = errorise-testing
TEMPLATE = app


SOURCES += main.cpp\
    tracks.cpp \
    mediadata.cpp \
    global.cpp \
    colorutils.cpp \
    translator.cpp \
    paths.cpp \
    preferences.cpp \
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
    interfaces/queueinterface.cpp \
    mplayer/mplayerpreferences.cpp \
    mplayer/mplayer.cpp \
    mplayer/mplayerversion.cpp \
    mplayer/mplayerprocess.cpp \
    mplayer/myprocess.cpp \
    phononface.cpp

HEADERS  += \
    tracks.h \
    mediadata.h \
    config.h \
    global.h \
    preferences.h \
    colorutils.h \
    translator.h \
    paths.h \
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
    interfaces/playerinterface.h \
    mplayer/mplayerpreferences.h \
    mplayer/mplayer.h \
    mplayer/mplayerversion.h \
    mplayer/mplayerprocess.h \
    mplayer/myprocess.h \
    phonon/phononface.h \
    phononface.h

FORMS    += \
    preferenceswindow.ui
