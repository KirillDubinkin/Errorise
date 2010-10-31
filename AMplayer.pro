#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T18:51:22
#
#-------------------------------------------------

QT       += core gui

TARGET = AMplayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
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
    filters.cpp \
    recents.cpp \
    urlhistory.cpp \
    mediasettings.cpp \
    amplayer.cpp \
    version.cpp \
    core.cpp \
    mediainfo.cpp \
    preferenceswindow.cpp

HEADERS  += mainwindow.h \
    myprocess.h \
    tracks.h \
    mediadata.h \
    config.h \
    mplayerprocess.h \
    global.h \
    preferences.h \
    colorutils.h \
    mplayerversion.h \
    constants.h \
    translator.h \
    paths.h \
    filters.h \
    recents.h \
    urlhistory.h \
    mediasettings.h \
    amplayer.h \
    version.h \
    core.h \
    mediainfo.h \
    preferenceswindow.h

FORMS    += mainwindow.ui \
    preferenceswindow.ui
