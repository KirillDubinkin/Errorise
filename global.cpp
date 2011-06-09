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
    qDebug("Load player");
    player    = new PhononFace();

    qDebug("Load music library");
    mlib      = new MusicLibrary(pref->music_library_path, pref->files_filter);
}

void Global::global_end() {
    qDebug("Delete player");
    delete player;

    qDebug("Delete library");
    delete mlib;

    qDebug("Delete preferences");
    delete pref;
}


