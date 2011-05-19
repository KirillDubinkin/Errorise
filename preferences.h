#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/* Global settings */

#include <QString>
#include <QStringList>


class Preferences {

private:
        QString config_path;

public:
        Preferences();
	virtual ~Preferences();

        void reset();
	void save();
	void load();


        inline QString configPath() { return config_path; }


            //! General
        QString log_filter;
        bool log;
        bool save_log;
	bool close_on_finish;
        bool auto_add_to_playlist; //! Add files to open  playlist


            //! Player
        QString audio_output;
        int volume;
        bool mute;


            //! GUI
        QString language;
        QString iconset;
        QString translations_path;
        bool stay_on_top;


            //! Library
        QString music_library_path;
        QString files_filter;
        QString pl_art_filename;
        QStringList art_search_patterns;
        QStringList art_search_folders;
        int         lib_update_timeout;

};

#endif
