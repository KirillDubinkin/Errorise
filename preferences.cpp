#include "preferences.h"
#include "global.h"
#include "paths.h"

#include <QSettings>
#include <QFileInfo>
#include <QRegExp>
#include <QDir>
#include <QLocale>
#include <QTextCodec>
#include <QDebug>

using namespace Global;

Preferences::Preferences() {

	reset();


#ifndef NO_USE_INI_FILES
	load();
#endif

      //  setPalette();
}

Preferences::~Preferences() {
#ifndef NO_USE_INI_FILES
	save();
#endif

}


void Preferences::reset() {

    qDebug("Prefences::reset");

    /* *******
       General
       ******* */

        ao = "";

        volume = 50;
        mute = false;

        play_only_this = false;


    /* ***************
       Drives (CD/DVD)
       *************** */

/*	dvd_device = "";
        cdrom_device = "";

#ifndef Q_OS_WIN
        // Try to set default values
        if (QFile::exists("/dev/dvd")) dvd_device = "/dev/dvd";
        if (QFile::exists("/dev/cdrom")) cdrom_device = "/dev/cdrom";
#endif

#ifdef Q_OS_WIN
        enable_audiocd_on_windows = false;
#endif

        vcd_initial_title = 2; // Most VCD's start at title #2

#if DVDNAV_SUPPORT
        use_dvdnav = false;
#endif

*/
    /* ***********
       Performance
       *********** */

        priority = AboveNormal; // Option only for windows

        cache_for_files = 0;
        cache_for_streams = 1000;
/*	cache_for_dvds = 0; // not recommended to use cache for dvds
        cache_for_vcds = 1000;
        cache_for_audiocds = 1000;
        cache_for_tv = 3000;
*/


    /* ********
       Advanced
       ******** */

        log_amplayer = true;
        log_filter = ".*";
        save_amplayer_log = true;

        prefer_ipv4 = true;


    /* *********
       GUI stuff
       ********* */

        stay_on_top = false;

        language = "";
        iconset = "";

        close_on_finish = false;

        default_font = "";

        auto_add_to_playlist = true;
        add_to_playlist_consecutive_files = false;


        status_bar = true;
        status_bar_format = "%format% | %bitrate% | %samplerate% | %channels% | %playback_time% / %length%";
        window_title_format = "%artist% - %title%";


        main_stylesheet = "";


        res_main_width = 972;
        res_main_height = 720;

        res_pref_width = 802;
        res_pref_height = 586;

        x=100;
        y=100;



    /* ********
        Colors
       ******** */

        color_text = "";
        color_base = "";
        color_window = "";



    /* ********
       TV (dvb)
       ******** */
/*
        check_channels_conf_on_startup = true;
        initial_tv_deinterlace = MediaSettings::Yadif_1;
        last_dvb_channel = "";
        last_tv_channel = "";
*/

    /* ***********
       Directories
       *********** */

        music_library_path = QDir::homePath();
        files_filter = QString("*.mp3;*.wv;*.flac;*.ogg;*.dts;*.ape;*.m4a;*.mp4;*.ac3;*.wma");

//	last_dvd_directory="";


    /* **************
       Initial values
       ************** */

	initial_volume = 40;

	initial_audio_track = 1;

}

#ifndef NO_USE_INI_FILES
void Preferences::save() {
	qDebug("Preferences::save");

	QSettings * set = settings;
        set->setIniCodec(QTextCodec::codecForLocale());


    /* *******
       General
       ******* */

	set->beginGroup( "general");

        set->setValue("driver/audio_output", ao);
	set->setValue("volume", volume);
	set->setValue("mute", mute);

        set->setValue("play_only_this", play_only_this);

	set->endGroup(); // general




    /* ***********
       Performance
       *********** */

	set->beginGroup( "performance");

	set->setValue("priority", priority);

	set->setValue("cache_for_files", cache_for_files);
	set->setValue("cache_for_streams", cache_for_streams);

	set->endGroup(); // performance



    /* ********
       Advanced
       ******** */

	set->beginGroup( "advanced");

        set->setValue("log_amplayer", log_amplayer);
	set->setValue("log_filter", log_filter);
        set->setValue("save_amplayer_log", save_amplayer_log);

	set->setValue("prefer_ipv4", prefer_ipv4);

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

	set->setValue("stay_on_top", (int) stay_on_top);
	set->setValue("language", language);
	set->setValue("iconset", iconset);

	set->setValue("close_on_finish", close_on_finish);

	set->setValue("default_font", default_font);

    set->setValue("auto_add_to_playlist", auto_add_to_playlist);
    set->setValue("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files);


        set->setValue("status_bar", status_bar);
        set->setValue("status_bar_format", status_bar_format);
        set->setValue("window_title_format", window_title_format);

        set->setValue("main_stylesheet", main_stylesheet);


        set->setValue("res_main_width", res_main_width);
        set->setValue("res_main_height", res_main_height);

        set->setValue("res_pref_width", res_pref_width);
        set->setValue("res_pref_height", res_pref_height);

        set->setValue("window_x", x);
        set->setValue("window_y", y);


	set->endGroup(); // gui





    /* ******
       Colors
       ****** */
        set->beginGroup("colors");

        set->setValue("color_text", color_text);
        set->setValue("color_base", color_base);
        set->setValue("color_window", color_window);

        set->endGroup(); // colors








    /* ***********
       Directories
       *********** */

	set->beginGroup( "directories");
        set->setValue("music_library_path", music_library_path);
        set->setValue("files_filter", files_filter);
	set->endGroup(); // directories


    /* **************
       Initial values
       ************** */

	set->beginGroup( "defaults");

	set->setValue("initial_volume", initial_volume);
        set->setValue("initial_audio_track", initial_audio_track);

	set->endGroup(); // defaults


	set->sync();
}



void Preferences::load() {
        qDebug("Preferences::load");

        QSettings * set = settings;
        set->setIniCodec(QTextCodec::codecForLocale());


    /* *******
       General
       ******* */

        set->beginGroup( "general");

        ao = set->value("driver/audio_output", ao).toString();
	volume = set->value("volume", volume).toInt();
	mute = set->value("mute", mute).toBool();

        play_only_this = set->value("play_only_this", play_only_this).toBool();

	set->endGroup(); // general



    /* ***********
       Performance
       *********** */

	set->beginGroup( "performance");

	priority = set->value("priority", priority).toInt();

	cache_for_files = set->value("cache_for_files", cache_for_files).toInt();
	cache_for_streams = set->value("cache_for_streams", cache_for_streams).toInt();

	set->endGroup(); // performance



    /* ********
       Advanced
       ******** */

	set->beginGroup( "advanced");

       log_amplayer = set->value("log_amplayer", log_amplayer).toBool();
	log_filter = set->value("log_filter", log_filter).toString();
        save_amplayer_log = set->value("save_amplayer_log", save_amplayer_log).toBool();


	prefer_ipv4 = set->value("prefer_ipv4", prefer_ipv4).toBool();

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

        stay_on_top = set->value("stay_on_top", stay_on_top).toBool();

	language = set->value("language", language).toString();
	iconset= set->value("iconset", iconset).toString();

	close_on_finish = set->value("close_on_finish", close_on_finish).toBool();

	default_font = set->value("default_font", default_font).toString();


	auto_add_to_playlist = set->value("auto_add_to_playlist", auto_add_to_playlist).toBool();
	add_to_playlist_consecutive_files = set->value("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files).toBool();


        status_bar = set->value("status_bar", status_bar).toBool();
        status_bar_format = set->value("status_bar_format", status_bar_format).toString();
        window_title_format = set->value("window_title_format", window_title_format).toString();

        main_stylesheet = set->value("main_stylesheet", main_stylesheet).toString();



        res_main_width = set->value("res_main_width", res_main_width).toInt();
        res_main_height = set->value("res_main_height", res_main_height).toInt();

        res_pref_width = set->value("res_pref_width", res_pref_width).toInt();
        res_pref_height = set->value("res_pref_height", res_pref_height).toInt();


        x = set->value("window_x", x).toInt();
        y = set->value("window_y", y).toInt();

	set->endGroup(); // gui




    /* ******
       Colors
       ****** */

        set->beginGroup("colors");

        color_text = set->value("color_text", color_text).toString();
        color_base = set->value("color_base", color_base).toString();
        color_window = set->value("color_window", color_window).toString();

        set->endGroup(); // colors


    /* ***********
       Directories
       *********** */

	set->beginGroup( "directories");

        music_library_path = set->value("music_library_path", music_library_path).toString();
        files_filter = set->value("files_filter", files_filter).toString();
        set->endGroup(); // directories


    /* **************
       Initial values
       ************** */

	set->beginGroup( "defaults");

        initial_volume = set->value("initial_volume", initial_volume).toInt();
	initial_audio_track = set->value("initial_audio_track", initial_audio_track).toInt();

	set->endGroup(); // defaults


}

#endif // NO_USE_INI_FILES
