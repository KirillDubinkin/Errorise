#-------------------------------------------------
#
# Project created by QtCreator 2010-09-17T18:51:22
#
#-------------------------------------------------
QT       += phonon
QT       += sql
QT       += core gui

#CONFIG   += static
#CONFIG   += console

#TARGET   = Errorise-console
#TARGET   = Errorise
TARGET   = errorise-testing
TEMPLATE = app

TRANSLATIONS = errorise_ru.ts

SOURCES += main.cpp\
    global.cpp \
    translator.cpp \
    preferences.cpp \
    helper.cpp \
    SimpleGUI/simplegui.cpp \
    phononface.cpp \
    SimplePlaylist/simpleplaylist.cpp \
    SimplePlaylist/simpleplprefs.cpp \
    MediaLibrary/pmediainfo.cpp \
    MediaLibrary/musiclibrary.cpp \
    MediaLibrary/albumtree.cpp \
    SimpleToolbar/simpletoolbar.cpp \
    SimpleToolbar/simpletoolbarprefs.cpp \
    MediaLibrary/albumtreeprefs.cpp \
    MediaLibrary/albumtreeprefswidget.cpp \
    SimpleToolbar/simpetoolbarprefswidget.cpp \
    SimpleGUI/simpleguiprefs.cpp \
    SimplePlaylist/coversqueue.cpp \
    MediaLibrary/minfo.cpp \
    MediaLibrary/mediainfo.cpp \
    SimpleGUI/prefswidget.cpp \
    SimpleGUI/simpleguiprefswidget.cpp \
    loader.cpp \
    SimplePlaylist/simpleplaylistprefswidget.cpp \
    fileoperations.cpp

HEADERS  += \
    global.h \
    preferences.h \
    translator.h \
    version.h \
    helper.h \
    SimpleGUI/simplegui.h \
    phononface.h \
    SimplePlaylist/simpleplaylist.h \
    SimplePlaylist/simpleplaylistpreferences.h \
    SimplePlaylist/simpleplprefs.h \
    MediaLibrary/pmediainfo.h \
    MediaLibrary/musiclibrary.h \
    MediaLibrary/albumtree.h \
    SimpleToolbar/simpletoolbar.h \
    SimpleToolbar/simpletoolbarprefs.h \
    MediaLibrary/albumtreeprefs.h \
    MediaLibrary/albumtreeprefswidget.h \
    SimpleToolbar/simpetoolbarprefswidget.h \
    SimpleGUI/simpleguiprefs.h \
    SimplePlaylist/coversqueue.h \
    MediaLibrary/minfo.h \
    MediaLibrary/mediainfo.h \
    SimpleGUI/prefswidget.h \
    SimpleGUI/simpleguiprefswidget.h \
    loader.h \
    SimplePlaylist/simpleplaylistprefswidget.h \
    fileoperations.h

FORMS    += \
    MediaLibrary/albumtreeprefswidget.ui \
    SimpleToolbar/simpetoolbarprefswidget.ui \
    SimpleGUI/simpleguiprefswidget.ui \
    SimplePlaylist/simpleplaylistprefswidget.ui

RESOURCES += \
    resources.qrc
