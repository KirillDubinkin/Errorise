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

        enum PlaybackOrder {
            SequentialPlayback = 0,
            RepeatTrack        = 1,
            RepeatPlaylist     = 2,
            ShuffleTracks      = 3,
            ShuffleAlbums      = 4,
            RandomPlayback     = 5
        };

        void reset();
	void save();
	void load();


        inline QString configPath() { return config_path; }


            //! General
        int version;
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
        bool    use_pl_art;
        QStringList art_search_patterns;
        QStringList art_search_folders;
        int         lib_update_timeout;


            //! Playback
        PlaybackOrder playback_order;

};

#endif
