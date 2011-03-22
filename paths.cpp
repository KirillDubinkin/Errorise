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

