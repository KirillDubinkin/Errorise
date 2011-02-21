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
    mediainfo.cpp \
    preferenceswindow.cpp \
    version.cpp \
    helper.cpp \
    simplegui.cpp \
    simpletoolbar.cpp \
    simplealbumlist.cpp \
    phononface.cpp \
    SimplePlaylist/simpleplaylist.cpp \
    SimplePlaylist/simpleplprefs.cpp

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
    mediainfo.h \
    preferenceswindow.h \
    helper.h \
    simplegui.h \
    simpletoolbar.h \
    simplealbumlist.h \
    phonon/phononface.h \
    phononface.h \
    SimplePlaylist/simpleplaylist.h \
    SimplePlaylist/simpleplaylistpreferences.h \
    SimplePlaylist/simpleplprefs.h

FORMS    += \
    preferenceswindow.ui
