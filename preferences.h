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


#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

/* Global settings */

#include <QString>
#include <QStringList>
#include <QSize>
#include <QPalette>
//#include "config.h"
//#include "audioequalizerlist.h"
//#include "assstyles.h"

class Recents;
class URLHistory;
class Filters;

class Preferences {

public:
	enum OSD { None = 0, Seek = 1, SeekTimer = 2, SeekTimerTotal = 3 };
	enum OnTop { NeverOnTop = 0, AlwaysOnTop = 1, WhilePlayingOnTop = 2 };
	enum Priority { Realtime = 0, High = 1, AboveNormal = 2, Normal = 3,
                    BelowNormal = 4, Idle = 5 };
        enum WheelFunction { DoNothing = 1, Seeking = 2, Volume = 4, Zoom = 8,
                         ChangeSpeed = 16 };
	enum OptionState { Detect = -1, Disabled = 0, Enabled = 1 };

        Q_DECLARE_FLAGS(WheelFunctions, WheelFunction);

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

	QString mplayer_bin;
        QString ao; // audio output

        QString mediainfo_cli;

	// SMPlayer will remember all media settings for all videos.
	// This options allow to disable it:
	bool dont_remember_media_settings; 	// Will not remember anything
	bool dont_remember_time_pos;		// Will not remember time pos


	// Audio
	bool use_soft_vol;
	int softvol_max;
	bool use_hwac3; // -afm hwac3
//	bool use_audio_equalizer;

	// Global volume options
	bool global_volume;
	int volume;
	bool mute;


	// For the -mc option
	bool use_mc;
	double mc_value;

	// Misc
	int osd;
	int osd_delay; //<! Delay in ms to show the OSD.

	QString file_settings_method; //!< Method to be used for saving file settings

        bool play_only_this;


    /* ***************
       Drives (CD/DVD)
       *************** */

//	QString dvd_device;
//	QString cdrom_device;

/*#ifdef Q_OS_WIN
	bool enable_audiocd_on_windows;
#endif
*/
//	int vcd_initial_title;

/*#if DVDNAV_SUPPORT
	bool use_dvdnav; //!< Opens DVDs using dvdnav: instead of dvd:
#endif
*/

    /* ***********
       Performance
       *********** */

	int priority;
	int threads; //!< number of threads to use for decoding (-lavdopts threads <1-8>)

	int cache_for_files;
	int cache_for_streams;
//	int cache_for_dvds;
//	int cache_for_vcds;
//	int cache_for_audiocds;
//	int cache_for_tv;



    /* ********
       Advanced
       ******** */

#if USE_ADAPTER
	int adapter; //Screen for overlay. If -1 it won't be used.
#endif

#if USE_COLORKEY
	unsigned int color_key;
#endif

	bool use_mplayer_window;

	bool use_idx; //!< Use -idx

	// Let the user pass options to mplayer
	QString mplayer_additional_options;
	QString mplayer_additional_audio_filters;

	// Logs
	bool log_mplayer;
        bool log_amplayer;
	QString log_filter;
	bool verbose_log;
        bool save_amplayer_log;

    //mplayer log autosaving
    bool autosave_mplayer_log;
    QString mplayer_log_saveto;
    //mplayer log autosaving end

/*#if REPAINT_BACKGROUND_OPTION
	//! If true, mplayerlayer erases its background
	bool repaint_video_background; 
#endif
*/
	//! If true it will autoload edl files with the same name of the file
    //! to play
//	bool use_edl_files;

	//! Preferred connection method: ipv4 or ipv6
	bool prefer_ipv4;

	//! Windows only. If true, smplayer will pass short filenames to mplayer.
	//! To workaround a bug in mplayer.
	bool use_short_pathnames; 

	//! If true, smplayer will use the prefix pausing_keep_force to keep
	//! the pause on slave commands. This experimental prefix was added
	//! in mplayer svn r27665.
	bool use_pausing_keep_force;

	OptionState use_correct_pts; //!< Pass -correct-pts to mplayer

	QString actions_to_run; //!< List of actions to run every time a video loads.


	/* *********
	   GUI stuff
	   ********* */

	bool compact_mode;
	OnTop stay_on_top;
	int size_factor;

	int resize_method; 	//!< Mainwindow resize method

#if STYLE_SWITCHING
	QString style; 	//!< SMPlayer look
#endif

	// Function of mouse buttons:
	QString mouse_left_click_function;
	QString mouse_right_click_function;
	QString mouse_double_click_function;
	QString mouse_middle_click_function;
	QString mouse_xbutton1_click_function;
	QString mouse_xbutton2_click_function;
	int wheel_function;

	QFlags<WheelFunctions> wheel_function_cycle;

	bool wheel_function_seeking_reverse;

	// Configurable seeking
	int seeking1; // By default 10s
	int seeking2; // By default 1m
	int seeking3; // By default 10m
	int seeking4; // For mouse wheel, by default 30s

//	bool update_while_seeking;
#if ENABLE_DELAYED_DRAGGING	
	int time_slider_drag_delay;
#endif
#if SEEKBAR_RESOLUTION
	//! If true, seeking will be done using a
	//! percentage (with fractions) instead of time.
	bool relative_seeking;  
#endif

	QString language;
	QString iconset;

	//! Number of times to show the balloon remembering that the program
	//! is still running in the system tray.
	int balloon_count;

	//! If true, the position of the main window will be saved before
	//! entering in fullscreen and will restore when going back to
	//! window mode.
//	bool restore_pos_after_fullscreen;

	bool save_window_size_on_exit;

	//! Close the main window when a file or playlist finish
	bool close_on_finish;

	QString default_font;

	//!< Pause the current file when the main window is not visible
//	bool pause_when_hidden;

	//!< Allow frre movement of the video window
//	bool allow_video_movement;

	QString gui; //!< The name of the GUI to use

#if USE_MINIMUMSIZE
	int gui_minimum_width;
#endif
	QSize default_size; // Default size of the main window

#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
	bool hide_video_window_on_audio_files;
#endif

//	bool report_mplayer_crashes;

#if REPORT_OLD_MPLAYER
	bool reported_mplayer_is_old;
#endif

	bool auto_add_to_playlist; //!< Add files to open to playlist
	bool add_to_playlist_consecutive_files;


        // Formating displayed information
        bool status_bar;
        QString status_bar_format;
        QString window_title_format;


            // Stylesheets
        QString main_stylesheet;
        QString pl_stylesheet;


        bool pl_alternate_colors;
        bool pl_custom_colors;


        QStringList pl_columns_names;

        QStringList pl_columns_format;
        QStringList pl_columns_back;

        QStringList pl_columns_playing_format;
        QStringList pl_columns_playng_back;

        QStringList pl_columns_sizes;

        QStringList pl_columns_aligment;


        QStringList pl_art_search_pattern;


        QStringList pl_color_text;
        QStringList pl_color_back;
        QStringList pl_color_play_text;
        QStringList pl_color_play_back;


        bool pl_groups_labels;
        QString pl_groups_format;
        QString pl_groups_stylesheet;
        QString pl_groups_text_color;
        QString pl_groups_back_color;

        int pl_groups_aligment;
        int pl_group_height;


        int pl_row_height;

        bool pl_show_playing_time;

        bool pl_use_html;
        bool pl_use_groups;
        bool pl_auto_scroll;


        // Windows size
        int res_main_width;
        int res_main_height;

        int res_pref_width;
        int res_pref_height;

        int res_tree_width;


        bool recursive_dirs;


        int x, y;




    /* *********
        Colors
       ********* */

        QPalette palette;

        QString color_text;
        QString color_base;
        QString color_window;









    /* ********
       TV (dvb)
       ******** */
/*
	bool check_channels_conf_on_startup;
	int initial_tv_deinterlace;
	QString last_dvb_channel;
	QString last_tv_channel;
*/

    /* ***********
       Directories
       *********** */

        QString music_library_path;
        QString files_filter;


    /* **************
       Initial values
       ************** */
	int initial_volume;

//	AudioEqualizerList initial_audio_equalizer;

	bool initial_volnorm;

	int initial_audio_channels;
	int initial_stereo_mode;

	int initial_audio_track;


    /* ************
       MPlayer info
       ************ */

	int mplayer_detected_version; 	//!< Latest version of mplayer parsed

	//! Version of mplayer supplied by the user which will be used if
	//! the version can't be parsed from mplayer output
	int mplayer_user_supplied_version;


    /* *********
       Instances
       ********* */

	bool use_single_instance;
	int connection_port; // Manual port
	bool use_autoport;
	int autoport; // Port automatically chosen by Qt


    /* ****************
       Floating control
       **************** */

        int floating_control_margin;
        int floating_control_width;
	bool floating_control_animated;
	bool floating_display_in_compact_mode;
#ifndef Q_OS_WIN
	bool bypass_window_manager;
#endif


    /* *******
       History
       ******* */

	Recents * history_recents;
	URLHistory * history_urls;


    /* *******
       Filters
       ******* */
	Filters * filters;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Preferences::WheelFunctions)

#endif
