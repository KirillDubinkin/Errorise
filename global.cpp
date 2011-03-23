#include "global.h"
#include "preferences.h"

#include <QSettings>
#include "translator.h"
#include "paths.h"
#include <QApplication>
#include <QFile>
#include "version.h"

QSettings       * Global::settings   = 0;
Preferences     * Global::pref       = 0;
Translator      * Global::translator = 0;

MusicLibrary    * Global::mlib       = 0;
PhononFace      * Global::player     = 0;

using namespace Global;

void Global::global_init(const QString & config_path) {
	qDebug("global_init");

	// Translator
	translator = new Translator();

	// settings
	if (!config_path.isEmpty()) {
		Paths::setConfigPath(config_path);
	}

	if (Paths::iniPath().isEmpty()) {
		settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                 mycompany(), myplayerName().toLower());
	} else {
                QString filename = Paths::iniPath() + "/" + myplayerName().toLower() + ".ini";
		settings = new QSettings( filename, QSettings::IniFormat );
		qDebug("global_init: config file: '%s'", filename.toUtf8().data());

	}

	// Preferences
        pref      = new Preferences();

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

	delete settings;
	delete translator;
}


