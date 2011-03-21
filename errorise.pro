#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T18:51:22
#
#-------------------------------------------------
QT       += phonon
QT       += sql
QT       += core gui

#CONFIG  += static

TARGET = errorise-testing
TEMPLATE = app


SOURCES += main.cpp\
    mediadata.cpp \
    global.cpp \
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
    SimplePlaylist/simpleplprefs.cpp \
    SimplePlaylist/simpleplaylistprefswindow.cpp \
    MediaLibrary/pmediainfo.cpp \
    MediaLibrary/musiclibrary.cpp \
    MediaLibrary/albumtree.cpp

HEADERS  += \
    mediadata.h \
    global.h \
    preferences.h \
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
    SimplePlaylist/simpleplprefs.h \
    SimplePlaylist/simpleplaylistprefswindow.h \
    MediaLibrary/pmediainfo.h \
    MediaLibrary/musiclibrary.h \
    MediaLibrary/albumtree.h

FORMS    += \
    preferenceswindow.ui
