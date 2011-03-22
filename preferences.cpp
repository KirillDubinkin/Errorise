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

Preferences::Preferences()
{
    reset();
    load();
}


Preferences::~Preferences()
{
    save();
}


void Preferences::reset()
{
    ao = "";

    volume = 50;
    mute = false;

    priority = AboveNormal; // Option only for windows

    cache_for_files = 0;
    cache_for_streams = 1000;

    log = true;
    save_log = true;
    log_filter = ".*";


    stay_on_top = false;

    language = "";
    iconset = "";

    close_on_finish = false;

    auto_add_to_playlist = true;
    add_to_playlist_consecutive_files = false;

    status = true;
    status_text = "%format% | %bitrate% | %samplerate% | %channels% | %playback_time% / %length%";
    window_title = "%artist% - %title%";

    main_stylesheet = "";

    res_main_width = 972;
    res_main_height = 720;

    res_pref_width = 802;
    res_pref_height = 586;

    x=100;
    y=100;


    music_library_path = QDir::homePath();
    files_filter = QString("*.mp3;*.wv;*.flac;*.ogg;*.dts;*.ape;*.m4a;*.mp4;*.ac3;*.wma");

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

        set->setValue("log", log);
	set->setValue("log_filter", log_filter);
        set->setValue("save_log", save_log);

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

	set->setValue("stay_on_top", (int) stay_on_top);
	set->setValue("language", language);
	set->setValue("iconset", iconset);

	set->setValue("close_on_finish", close_on_finish);

    set->setValue("auto_add_to_playlist", auto_add_to_playlist);
    set->setValue("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files);


        set->setValue("show_status", status);
        set->setValue("status_text", status_text);
        set->setValue("window_title", window_title);

        set->setValue("main_stylesheet", main_stylesheet);


        set->setValue("res_main_width", res_main_width);
        set->setValue("res_main_height", res_main_height);

        set->setValue("res_pref_width", res_pref_width);
        set->setValue("res_pref_height", res_pref_height);

        set->setValue("window_x", x);
        set->setValue("window_y", y);


	set->endGroup(); // gui


    /* ***********
       Directories
       *********** */

	set->beginGroup( "directories");
        set->setValue("music_library_path", music_library_path);
        set->setValue("files_filter", files_filter);
	set->endGroup(); // directories

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

        log = set->value("log", log).toBool();
	log_filter = set->value("log_filter", log_filter).toString();
        save_log = set->value("save_log", save_log).toBool();

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

        stay_on_top = set->value("stay_on_top", stay_on_top).toBool();

	language = set->value("language", language).toString();
	iconset= set->value("iconset", iconset).toString();

	close_on_finish = set->value("close_on_finish", close_on_finish).toBool();

	auto_add_to_playlist = set->value("auto_add_to_playlist", auto_add_to_playlist).toBool();
	add_to_playlist_consecutive_files = set->value("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files).toBool();


        status = set->value("show_status", status).toBool();
        status_text = set->value("status_text", status_text).toString();
        window_title = set->value("window_title", window_title).toString();

        main_stylesheet = set->value("main_stylesheet", main_stylesheet).toString();


        res_main_width = set->value("res_main_width", res_main_width).toInt();
        res_main_height = set->value("res_main_height", res_main_height).toInt();

        res_pref_width = set->value("res_pref_width", res_pref_width).toInt();
        res_pref_height = set->value("res_pref_height", res_pref_height).toInt();


        x = set->value("window_x", x).toInt();
        y = set->value("window_y", y).toInt();

	set->endGroup(); // gui


    /* ***********
       Directories
       *********** */

	set->beginGroup( "directories");

        music_library_path = set->value("music_library_path", music_library_path).toString();
        files_filter = set->value("files_filter", files_filter).toString();
        set->endGroup(); // directories

}

#endif // NO_USE_INI_FILES
