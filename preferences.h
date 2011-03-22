#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/* Global settings */

#include <QString>
#include <QStringList>
#include <QSize>
#include <QPalette>


class Preferences {

public:
	enum Priority { Realtime = 0, High = 1, AboveNormal = 2, Normal = 3,
                    BelowNormal = 4, Idle = 5 };

	Preferences();
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

	int priority;

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


        bool status;
        QString status_text;
        QString window_title;

        QString main_stylesheet;


        // Windows size
        int res_main_width;
        int res_main_height;

        int res_pref_width;
        int res_pref_height;

        int x, y;

        QString music_library_path;
        QString files_filter;

};

#endif
