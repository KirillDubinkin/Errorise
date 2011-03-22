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

#include "paths.h"
#include <QLibraryInfo>
#include <QLocale>
#include <QFile>
#include <QRegExp>
#include <QDir>

#ifndef Q_OS_WIN
#include <stdlib.h>
#endif

#include "version.h"

QString Paths::app_path;
QString Paths::config_path;


void Paths::setAppPath(QString path) {
	app_path = path;
}


QString Paths::appPath() {
	return app_path;
}


QString Paths::translationPath() {
#ifdef TRANSLATION_PATH
	QString path = QString(TRANSLATION_PATH);
	if (!path.isEmpty())
		return path;
	else
		return appPath() + "/translations";
#else
	return appPath() + "/translations";
#endif
}


QString Paths::qtTranslationPath() {
	return QLibraryInfo::location(QLibraryInfo::TranslationsPath);
}



void Paths::setConfigPath(QString path) {
	config_path = path;
}


QString Paths::configPath() {
	if (!config_path.isEmpty()) {
		return config_path;
	} else {
#ifdef PORTABLE_APP
		return appPath();
#else
		#ifndef Q_OS_WIN
		const char * XDG_CONFIG_HOME = getenv("XDG_CONFIG_HOME");
		if (XDG_CONFIG_HOME!=NULL) {
			qDebug("Paths::configPath: XDG_CONFIG_HOME: %s", XDG_CONFIG_HOME);
                        return QString(XDG_CONFIG_HOME) + "/" + myplayerName().toLower();
		} 
		else
                return QDir::homePath() + "/.config/" + myplayerName().toLower();
		#else
                return "%APPDATA%/" + myplayerName();
		#endif

#endif
	}
}


QString Paths::iniPath() {
	return configPath();
}

