#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <QString>
#include "preferences.h"

#include "phononface.h"
#include "MediaLibrary/musiclibrary.h"

// Some global objects


class QSettings;
class Preferences;
class Translator;


class PhononFace;
class MusicLibrary;

namespace Global {

        //! Read and store application settings
	extern QSettings * settings;

	//! Prefences
	extern Preferences * pref;

	//! Translator (for changing language)
	extern Translator * translator;


        extern MusicLibrary *mlib;
        extern PhononFace   *player;


	void global_init(const QString & config_path);
	void global_end();

};

#endif

