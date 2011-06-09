#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <QString>
#include "preferences.h"

#include "phononface.h"
#include "MediaLibrary/musiclibrary.h"

// Some global objects

class Preferences;
class PhononFace;
class MusicLibrary;
class SimpleGUI;

namespace Global {

	//! Prefences
	extern Preferences * pref;
        extern MusicLibrary *mlib;
        extern PhononFace   *player;
        extern SimpleGUI    *gui;

        void init_preferences();
        void global_init();
	void global_end();

}

#endif
