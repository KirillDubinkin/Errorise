#ifndef _PATHS_H_
#define _PATHS_H_

#include <QString>

class Paths {

public:

	static void setAppPath(QString path);
	static QString appPath();

	static QString translationPath();
        static QString qtTranslationPath();


	//! Forces to use a different path for the config files
	static void setConfigPath(QString path);

        //! Return the path where player should save its config files
	static QString configPath();

	//! Obsolete. Just returns configPath()
	static QString iniPath();

private:
	static QString app_path;
	static QString config_path;
};

#endif
