#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <QString>
#include "preferences.h"

#include "phononface.h"
#include "MediaLibrary/musiclibrary.h"

// Some global objects

class Preferences;
//class Translator;

class PhononFace;
class MusicLibrary;
class SimpleGUI;

namespace Global {

	//! Prefences
	extern Preferences * pref;

	//! Translator (for changing language)
//	extern Translator * translator;

        extern MusicLibrary *mlib;
        extern PhononFace   *player;
        extern SimpleGUI    *gui;

        void global_init();
	void global_end();

};

#endif
