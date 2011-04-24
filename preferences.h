#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/* Global settings */

#include <QString>


class Preferences {

private:
        QString filename;

public:
        Preferences(QString filename = QString::null);
	virtual ~Preferences();

	virtual void reset();

#ifndef NO_USE_INI_FILES
	void save();
	void load();
#endif

    /* *******
       General
       ******* */
        QString ao; // audio output
        QString log_filter;

	int volume;
	bool mute;

	int cache_for_files;
	int cache_for_streams;

        bool log;
        bool save_log;


        bool stay_on_top;

	QString language;
	QString iconset;

	bool close_on_finish;

	bool auto_add_to_playlist; //!< Add files to open to playlist
	bool add_to_playlist_consecutive_files;


        QString music_library_path;
        QString files_filter;


//////// -  It's goes to GUI preferences  - ///////
        // Windows size
        int res_main_width;
        int res_main_height;

        int x, y;
/////////////////////////////////////////////////

};

#endif
