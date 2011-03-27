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
    global.cpp \
    translator.cpp \
    paths.cpp \
    preferences.cpp \
    amplayer.cpp \
    helper.cpp \
    simplegui.cpp \
    phononface.cpp \
    SimplePlaylist/simpleplaylist.cpp \
    SimplePlaylist/simpleplprefs.cpp \
    SimplePlaylist/simpleplaylistprefswindow.cpp \
    MediaLibrary/pmediainfo.cpp \
    MediaLibrary/musiclibrary.cpp \
    MediaLibrary/albumtree.cpp \
    SimpleToolbar/simpletoolbar.cpp \
    SimpleToolbar/simpletoolbarprefs.cpp \
    MediaLibrary/albumtreeprefs.cpp \
    MediaLibrary/albumtreeprefswidget.cpp \
    SimpleToolbar/simpetoolbarprefswidget.cpp

HEADERS  += \
    global.h \
    preferences.h \
    translator.h \
    paths.h \
    amplayer.h \
    version.h \
    helper.h \
    simplegui.h \
    phonon/phononface.h \
    phononface.h \
    SimplePlaylist/simpleplaylist.h \
    SimplePlaylist/simpleplaylistpreferences.h \
    SimplePlaylist/simpleplprefs.h \
    SimplePlaylist/simpleplaylistprefswindow.h \
    MediaLibrary/pmediainfo.h \
    MediaLibrary/musiclibrary.h \
    MediaLibrary/albumtree.h \
    SimpleToolbar/simpletoolbar.h \
    SimpleToolbar/simpletoolbarprefs.h \
    MediaLibrary/albumtreeprefs.h \
    MediaLibrary/albumtreeprefswidget.h \
    SimpleToolbar/simpetoolbarprefswidget.h

FORMS    += \
    MediaLibrary/albumtreeprefswidget.ui \
    SimpleToolbar/simpetoolbarprefswidget.ui
