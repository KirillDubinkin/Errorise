/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "global.h"
#include "preferences.h"
#include "mediainfo.h"

#ifndef MINILIB

#include <QSettings>
#include "translator.h"
#include "paths.h"
#include <QApplication>
#include <QFile>
#include "version.h"

QSettings       * Global::settings   = 0;
Preferences     * Global::pref       = 0;
Translator      * Global::translator = 0;
MediaInfo       * Global::mediainfo  = 0;

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
        mediainfo = new MediaInfo();

        player    = new PhononFace();
        mlib      = new MusicLibrary(pref->music_library_path,
                                     pref->files_filter);


}

void Global::global_end() {
	qDebug("global_end");

        // delete

        delete player;
        delete mlib;

        delete mediainfo;
	delete pref;
	pref = 0;

	delete settings;
	delete translator;
}

#else

Preferences * Global::pref = 0;

using namespace Global;

void Global::global_init() {
	qDebug("global_init");

	// Preferences
	pref = new Preferences();
}

void Global::global_end() {
	qDebug("global_end");

	// delete
	delete pref;
	pref = 0;
}

#endif // MINILIB

