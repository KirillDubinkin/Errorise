#include "global.h"
#include "preferences.h"

#include <QApplication>
#include <QFile>

Preferences  * Global::pref       = 0;
MusicLibrary * Global::mlib       = 0;
PhononFace   * Global::player     = 0;
SimpleGUI    * Global::gui        = 0;

using namespace Global;

void Global::init_preferences()
{
    qDebug("Load global preferences");
    pref = new Preferences();
}

void Global::global_init()
{
    qDebug("Load Phonon");
    player    = new PhononFace();

    qDebug("Load Music library");
    mlib      = new MusicLibrary(pref->music_library_path, pref->files_filter);
}

void Global::global_end() {
    qDebug("Delete Phonon");
    delete player;

    qDebug("Delete Music library");
    delete mlib;

    qDebug("Delete global preferences");
    delete pref;
}


