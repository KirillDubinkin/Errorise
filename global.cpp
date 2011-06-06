#include "global.h"
#include "preferences.h"

//#include "translator.h"
#include <QApplication>
#include <QFile>
//#include "version.h"

Preferences  * Global::pref       = 0;
//Translator * Global::translator = 0;

MusicLibrary * Global::mlib       = 0;
PhononFace   * Global::player     = 0;
SimpleGUI    * Global::gui        = 0;

using namespace Global;

void Global::global_init()
{
	qDebug("global_init");

	// Preferences
        pref      = new Preferences();

        // Translator
//        translator = new Translator();

        player    = new PhononFace();
        mlib      = new MusicLibrary(pref->music_library_path,
                                     pref->files_filter);
}

void Global::global_end() {
	qDebug("global_end");

        // delete

        delete player;
        delete mlib;

	delete pref;
	pref = 0;

//	delete translator;
}


