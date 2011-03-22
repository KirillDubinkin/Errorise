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
        void setPalette();
        QString getHex(int r, int g, int b);


    /* *******
       General
       ******* */
        QString ao; // audio output
        QString log_filter;

//	bool dont_remember_media_settings; 	// Will not remember anything
//	bool dont_remember_time_pos;		// Will not remember time pos



	// Global volume options
	int volume;
	bool mute;

        bool play_only_this;



    /* ***********
       Performance
       *********** */

	int priority;

	int cache_for_files;
	int cache_for_streams;


    /* ********
       Advanced
       ******** */

        bool log_amplayer;
        bool save_amplayer_log;

	//! Preferred connection method: ipv4 or ipv6
	bool prefer_ipv4;


	/* *********
	   GUI stuff
	   ********* */

        bool stay_on_top;

	QString language;
	QString iconset;

	bool close_on_finish;

	QString default_font;


	bool auto_add_to_playlist; //!< Add files to open to playlist
	bool add_to_playlist_consecutive_files;


        // Formating displayed information
        bool status_bar;
        QString status_bar_format;
        QString window_title_format;


        // Stylesheet
        QString main_stylesheet;


        // Windows size
        int res_main_width;
        int res_main_height;

        int res_pref_width;
        int res_pref_height;

        int x, y;


    /* *********
        Colors
       ********* */

        QPalette palette;

        QString color_text;
        QString color_base;
        QString color_window;



    /* ***********
       Directories
       *********** */

        QString music_library_path;
        QString files_filter;


    /* **************
       Initial values
       ************** */
	int initial_volume;
	int initial_audio_track;


};

#endif
