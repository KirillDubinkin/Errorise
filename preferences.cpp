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


void Preferences::setPalette()
{
    bool ok;

    if (!color_text.isEmpty())
        palette.setColor(QPalette::Text, QColor(color_text.toInt(&ok, 16)));

    if (!color_base.isEmpty())
        palette.setColor(QPalette::Base, QColor(color_base.toInt(&ok, 16)));

    if (!color_window.isEmpty())
        palette.setColor(QPalette::Window, QColor(color_window.toInt(&ok, 16)));

//    int col = this->pl_columns_names.size();
/*
    for (int i = 0; i < col; i++){
        //qDebug() << pl_color_back.at(i);
        if (i <= pl_color_back.size())
            if (QString(pl_color_back.at(i)).isEmpty()){
                int r=0, g=0, b=0;
                palette.base().color().getRgb(&r, &g, &b);
                pl_color_back.replace(i, getHex(r, g, b));
             //   qDebug() << pl_color_back.at(i);
        }

        if (i <= pl_color_text.size())
            if (QString(pl_color_text.at(i)).isEmpty()){
                int r=0, g=0, b=0;
                palette.text().color().getRgb(&r, &g, &b);
                pl_color_text.replace(i, getHex(r, g, b));
        }
    }

    if (this->pl_groups_back_color.isEmpty()){
        int r=0, g=0, b=0;
        palette.base().color().getRgb(&r, &g, &b);
        pl_groups_back_color = getHex(r, g, b);
    }

    if (this->pl_groups_text_color.isEmpty()){
        int r=0, g=0, b=0;
        palette.text().color().getRgb(&r, &g, &b);
        pl_groups_text_color = getHex(r, g, b);
    }
*/
   // qDebug() << pl_color_back.at(1);
}


QString Preferences::getHex(int r, int g, int b)
{
    QString str = "";
    if (r < 16) str = "0";
    str += QString().number(r,16);
    if (g < 16) str += "0";
    str += QString().number(g,16);
    if (b < 16) str += "0";
    str += QString().number(b,16);

    return str;
}


void Preferences::reset() {

    qDebug("Prefences::reset");

    /* *******
       General
       ******* */

#ifdef Q_OS_WIN
        mplayer_bin= "mplayer.exe";
#else
        mplayer_bin = "mplayer";
#endif
        ao = "";

#ifdef Q_OS_WIN
        mediainfo_cli = "mediainfo.exe";
#else
        mediainfo_cli = "mediainfo";
#endif


        dont_remember_media_settings = false;
        dont_remember_time_pos = false;

        use_soft_vol = true;
        softvol_max = 110; // 110 = default value in mplayer

        use_hwac3 = false;
//	use_audio_equalizer = true;

        global_volume = true;
        volume = 50;
        mute = false;

        use_mc = false;
        mc_value = 0;

        osd = Seek;
        osd_delay = 2200;

        file_settings_method = "hash"; // Possible values: normal & hash

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

        threads = 1;

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

#if USE_ADAPTER
        adapter = -1;
#endif

#if USE_COLORKEY
        color_key = 0x020202;
#endif

        use_mplayer_window = false;

        use_idx = false;

        mplayer_additional_options="";
        #ifdef PORTABLE_APP
        mplayer_additional_options="-nofontconfig";
        #endif

    mplayer_additional_audio_filters="";

        log_mplayer = true;
        log_amplayer = true;
        log_filter = ".*";
        verbose_log = false;
        save_amplayer_log = true;

    //mplayer log autosaving
    autosave_mplayer_log = false;
    mplayer_log_saveto = "";
    //mplayer log autosaving end


//	use_edl_files = true;
        prefer_ipv4 = true;

        use_short_pathnames = false;

        use_pausing_keep_force = true;

        use_correct_pts = Detect;

        actions_to_run = "";


    /* *********
       GUI stuff
       ********* */

        compact_mode = false;
        stay_on_top = NeverOnTop;
        size_factor = 100; // 100%

//	resize_method = Always;

#if STYLE_SWITCHING
        style="";
#endif

#if DVDNAV_SUPPORT
        mouse_left_click_function = "dvdnav_mouse";
#else
        mouse_left_click_function = "";
#endif
        mouse_right_click_function = "show_context_menu";
        mouse_double_click_function = "fullscreen";
        mouse_middle_click_function = "mute";
        mouse_xbutton1_click_function = "";
        mouse_xbutton2_click_function = "";
        wheel_function = Seeking;
        wheel_function_cycle = Seeking | Volume | Zoom | ChangeSpeed;
        wheel_function_seeking_reverse = false;

        seeking1 = 10;
        seeking2 = 60;
        seeking3 = 10*60;
        seeking4 = 30;

#if ENABLE_DELAYED_DRAGGING
        time_slider_drag_delay = 100;
#endif
#if SEEKBAR_RESOLUTION
        relative_seeking = true;
#endif

        language = "";
        iconset = "";

        balloon_count = 5;

        save_window_size_on_exit = true;

        close_on_finish = false;

        default_font = "";

//	pause_when_hidden = false;

//	allow_video_movement = false;

        gui = "DefaultGui";

#if USE_MINIMUMSIZE
        gui_minimum_width = 0; // 0 == disabled
#endif
        default_size = QSize(580, 440);

#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
        hide_video_window_on_audio_files = true;
#endif

//	report_mplayer_crashes = true;

#if REPORT_OLD_MPLAYER
        reported_mplayer_is_old = false;
#endif

        auto_add_to_playlist = true;
        add_to_playlist_consecutive_files = false;


        status_bar = true;
        status_bar_format = "%format% | %bitrate% | %samplerate% | %channels% | %playback_time% / %length%";
        window_title_format = "%artist% - %title%";


        main_stylesheet = "";
        pl_stylesheet = "selection-background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.681, fx:0.5, fy:0.5, stop:0 rgba(91, 0, 115, 255), stop:1 rgba(175, 43, 202, 255));\nselection-color: rgb(255, 255, 255);\n";

        pl_alternate_colors = true;
        pl_custom_colors = false;



        pl_columns_names << "Cover" << "#" << "Length" << "Track Name" << "Bitrate" << "Format";

        pl_columns_format << "%art%" << "%tracknumber%" << "%length%" << "%title%" << "%bitrate%" << "%codec%";
        pl_columns_back << "" << "" << "" << "" << "" << "";

        pl_columns_playing_format << "%art%" << "%tracknumber%" << "%length%" << "%title%" << "%bitrate%" << "%codec%";
        pl_columns_playng_back << "" << "" << "" << "" << "" << "";

        pl_columns_sizes << "200" << "23" << "75" << "200" << "75" << "50";
        pl_columns_aligment << "4" << "4" << "4" << "4" << "4" << "4";

        pl_art_search_pattern << "*cover*.jpg" << "*folder*.jpg" << "*front*.jpg";



        pl_color_text << "" << "527482" << "43606b" << "" << "43606b" << "527482";
        pl_color_back << "" << "" << "" << "" << "" << "";

        pl_color_play_text << "" << "" << "" << "" << "" << "";
        pl_color_play_back << "" << "" << "" << "" << "" << "";




        pl_groups_labels = true;
        pl_groups_format = "[%date%] %album%";
        pl_groups_stylesheet = "background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.681, fx:0.5, fy:0.5, stop:0 rgba(0, 35, 51, 255), stop:1 rgba(0, 74, 92, 255));\ncolor: rgb(255, 255, 255);";
        pl_groups_text_color = "ffffff";
        pl_groups_back_color = "13363b";
        pl_groups_aligment = 0x1;
        pl_group_height = 20;


        pl_row_height = 14;

        pl_show_playing_time = false;

        pl_use_html = false;
        pl_use_groups = true;
        pl_auto_scroll = true;


        res_main_width = 972;
        res_main_height = 720;

        res_pref_width = 802;
        res_pref_height = 586;

        res_tree_width = 300;

        recursive_dirs = true;

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

//	initial_audio_equalizer << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;

        initial_volnorm = false;

	initial_audio_track = 1;

    /* ************
       MPlayer info
       ************ */

	mplayer_detected_version = -1; //None version parsed yet
	mplayer_user_supplied_version = -1;


    /* *********
       Instances
       ********* */

#ifdef Q_OS_WIN
	// Some people reported smplayer doesn't start with this option enabled
	// So now it's disabled by default on Windows
	use_single_instance = false; 
#else
	use_single_instance = true;
#endif
	use_autoport = true;
	connection_port = 8000;
	autoport = 0;


    /* ****************
       Floating control
       **************** */

	floating_control_margin = 0;
//	floating_control_width = 100; //100 %
	floating_control_animated = true;
	floating_display_in_compact_mode = false;
#ifndef Q_OS_WIN
	bypass_window_manager = true;
#endif



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

	set->setValue("mplayer_bin", mplayer_bin);
        set->setValue("driver/audio_output", ao);
        set->setValue("mediainfo_cli", mediainfo_cli);

	set->setValue("dont_remember_media_settings", dont_remember_media_settings);
	set->setValue("dont_remember_time_pos", dont_remember_time_pos);

	set->setValue("use_soft_vol", use_soft_vol);
	set->setValue("softvol_max", softvol_max);
	set->setValue("use_hwac3", use_hwac3 );

	set->setValue("global_volume", global_volume);
	set->setValue("volume", volume);
	set->setValue("mute", mute);

	set->setValue("use_mc", use_mc);
	set->setValue("mc_value", mc_value);

	set->setValue("osd", osd);
	set->setValue("osd_delay", osd_delay);

	set->setValue("file_settings_method", file_settings_method);

        set->setValue("play_only_this", play_only_this);

	set->endGroup(); // general


    /* ***************
       Drives (CD/DVD)
       *************** */
/*
	set->beginGroup( "drives");

	set->setValue("dvd_device", dvd_device);
	set->setValue("cdrom_device", cdrom_device);

#ifdef Q_OS_WIN
	set->setValue("enable_audiocd_on_windows", enable_audiocd_on_windows);
#endif

	set->setValue("vcd_initial_title", vcd_initial_title);

#if DVDNAV_SUPPORT
	set->setValue("use_dvdnav", use_dvdnav);
#endif

	set->endGroup(); // drives
*/


    /* ***********
       Performance
       *********** */

	set->beginGroup( "performance");

	set->setValue("priority", priority);
//	set->setValue("fast_audio_change", fast_audio_change);
	set->setValue("threads", threads);

	set->setValue("cache_for_files", cache_for_files);
	set->setValue("cache_for_streams", cache_for_streams);
/*	set->setValue("cache_for_dvds", cache_for_dvds);
	set->setValue("cache_for_vcds", cache_for_vcds);
	set->setValue("cache_for_audiocds", cache_for_audiocds);
	set->setValue("cache_for_tv", cache_for_tv);
*/
	set->endGroup(); // performance



    /* ********
       Advanced
       ******** */

	set->beginGroup( "advanced");

#if USE_ADAPTER
	set->setValue("adapter", adapter);
#endif

#if USE_COLORKEY
	set->setValue("color_key", QString::number(color_key,16));
#endif

//	set->setValue("use_idx", use_idx);

	set->setValue("mplayer_additional_options", mplayer_additional_options);
	set->setValue("mplayer_additional_audio_filters", mplayer_additional_audio_filters);

	set->setValue("log_mplayer", log_mplayer);
        set->setValue("log_amplayer", log_amplayer);
	set->setValue("log_filter", log_filter);
	set->setValue("verbose_log", verbose_log);
        set->setValue("save_amplayer_log", save_amplayer_log);

    //mplayer log autosaving
    set->setValue("autosave_mplayer_log", autosave_mplayer_log);
    set->setValue("mplayer_log_saveto", mplayer_log_saveto);
    //mplayer log autosaving end

//	set->setValue("use_edl_files", use_edl_files);

	set->setValue("prefer_ipv4", prefer_ipv4);

//	set->setValue("use_short_pathnames", use_short_pathnames);

//	set->setValue("use_pausing_keep_force", use_pausing_keep_force);

//	set->setValue("correct_pts", use_correct_pts);

//	set->setValue("actions_to_run", actions_to_run);

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

	set->setValue("compact_mode", compact_mode);
	set->setValue("stay_on_top", (int) stay_on_top);
	set->setValue("size_factor", size_factor);
	set->setValue("resize_method", resize_method);

#if STYLE_SWITCHING
	set->setValue("style", style);
#endif

	set->setValue("mouse_left_click_function", mouse_left_click_function);
	set->setValue("mouse_right_click_function", mouse_right_click_function);
	set->setValue("mouse_double_click_function", mouse_double_click_function);
	set->setValue("mouse_middle_click_function", mouse_middle_click_function);
	set->setValue("mouse_xbutton1_click_function", mouse_xbutton1_click_function);
	set->setValue("mouse_xbutton2_click_function", mouse_xbutton2_click_function);
	set->setValue("mouse_wheel_function", wheel_function);
	set->setValue("wheel_function_cycle", (int) wheel_function_cycle);
	set->setValue("wheel_function_seeking_reverse", wheel_function_seeking_reverse);

	set->setValue("seeking1", seeking1);
	set->setValue("seeking2", seeking2);
	set->setValue("seeking3", seeking3);
	set->setValue("seeking4", seeking4);

//	set->setValue("update_while_seeking", update_while_seeking);
#if ENABLE_DELAYED_DRAGGING
	set->setValue("time_slider_drag_delay", time_slider_drag_delay);
#endif
#if SEEKBAR_RESOLUTION
	set->setValue("relative_seeking", relative_seeking);
#endif

	set->setValue("language", language);
	set->setValue("iconset", iconset);

	set->setValue("balloon_count", balloon_count);

	set->setValue("save_window_size_on_exit", save_window_size_on_exit);

	set->setValue("close_on_finish", close_on_finish);

	set->setValue("default_font", default_font);

//	set->setValue("pause_when_hidden", pause_when_hidden);

	set->setValue("gui", gui);

#if USE_MINIMUMSIZE
	set->setValue("gui_minimum_width", gui_minimum_width);
#endif
	set->setValue("default_size", default_size);
/*
#if ALLOW_TO_HIDE_VIDEO_WINDOW_ON_AUDIO_FILES
	set->setValue("hide_video_window_on_audio_files", hide_video_window_on_audio_files);
#endif
*/
//	set->setValue("report_mplayer_crashes", report_mplayer_crashes);

#if REPORT_OLD_MPLAYER
	set->setValue("reported_mplayer_is_old", reported_mplayer_is_old);
#endif

    set->setValue("auto_add_to_playlist", auto_add_to_playlist);
    set->setValue("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files);


        set->setValue("status_bar", status_bar);
        set->setValue("status_bar_format", status_bar_format);
        set->setValue("window_title_format", window_title_format);

        set->setValue("main_stylesheet", main_stylesheet);
        set->setValue("pl_alternate_colors", pl_alternate_colors);
        set->setValue("pl_custom_colors", pl_custom_colors);


        set->setValue("pl_columns_names", pl_columns_names);
        set->setValue("pl_columns_format", pl_columns_format);
        set->setValue("pl_columns_back", pl_columns_back);
        set->setValue("pl_columns_playing_format", pl_columns_playing_format);
        set->setValue("pl_columns_playng_back", pl_columns_playng_back);
        set->setValue("pl_columns_sizes", pl_columns_sizes);
        set->setValue("pl_columns_aligment", pl_columns_aligment);

        set->setValue("pl_art_search_pattern", pl_art_search_pattern);

        set->setValue("pl_color_text", pl_color_text);
        set->setValue("pl_color_back", pl_color_back);
        set->setValue("pl_color_play_text", pl_color_play_text);
        set->setValue("pl_color_play_back", pl_color_play_back);

        set->setValue("pl_stylesheet", pl_stylesheet);

        set->setValue("pl_groups_labels", pl_groups_labels);
        set->setValue("pl_groups_format", pl_groups_format);
        set->setValue("pl_groups_stylesheet", pl_groups_stylesheet);
        set->setValue("pl_groups_text_color", pl_groups_text_color);
        set->setValue("pl_groups_back_color", pl_groups_back_color);
        set->setValue("pl_groups_aligment", pl_groups_aligment);
        set->setValue("pl_group_height", pl_group_height);


        set->setValue("pl_row_height", pl_row_height);

        set->setValue("pl_show_playing_time", pl_show_playing_time);
        set->setValue("pl_use_html", pl_use_html);
        set->setValue("pl_use_groups", pl_use_groups);
        set->setValue("pl_auto_scroll", pl_auto_scroll);


        set->setValue("res_main_width", res_main_width);
        set->setValue("res_main_height", res_main_height);

        set->setValue("res_pref_width", res_pref_width);
        set->setValue("res_pref_height", res_pref_height);

        set->setValue("res_tree_width", res_tree_width);

        set->setValue("recursive_dirs", recursive_dirs);

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
//	set->setValue("last_dvd_directory", last_dvd_directory);
	set->endGroup(); // directories


    /* **************
       Initial values
       ************** */

	set->beginGroup( "defaults");

	set->setValue("initial_volume", initial_volume);
//	set->setValue("initial_audio_equalizer", initial_audio_equalizer);
        set->setValue("initial_volnorm", initial_volnorm);
	set->setValue("initial_audio_channels", initial_audio_channels);
	set->setValue("initial_stereo_mode", initial_stereo_mode);
        set->setValue("initial_audio_track", initial_audio_track);

	set->endGroup(); // defaults


    /* ************
       MPlayer info
       ************ */

	set->beginGroup( "mplayer_info");
	set->setValue("mplayer_detected_version", mplayer_detected_version);
	set->setValue("mplayer_user_supplied_version", mplayer_user_supplied_version);
	set->endGroup(); // mplayer_info


    /* *********
       Instances
       ********* */

	set->beginGroup("instances");
	set->setValue("use_single_instance", use_single_instance);
	set->setValue("connection_port", connection_port);
	set->setValue("use_autoport", use_autoport);
	set->setValue("temp/autoport", autoport);
	set->endGroup(); // instances


    /* ****************
       Floating control
       **************** */

	set->beginGroup("floating_control");
	set->setValue("margin", floating_control_margin);
        set->setValue("width", floating_control_width);
	set->setValue("animated", floating_control_animated);
	set->setValue("display_in_compact_mode", floating_display_in_compact_mode);
#ifndef Q_OS_WIN
	set->setValue("bypass_window_manager", bypass_window_manager);
#endif
	set->endGroup(); // floating_control


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

        mplayer_bin = set->value("mplayer_bin", mplayer_bin).toString();
        ao = set->value("driver/audio_output", ao).toString();
        mediainfo_cli = set->value("mediainfo_cli", mediainfo_cli).toString();

	dont_remember_media_settings = set->value("dont_remember_media_settings", dont_remember_media_settings).toBool();
	dont_remember_time_pos = set->value("dont_remember_time_pos", dont_remember_time_pos).toBool();

	use_soft_vol = set->value("use_soft_vol", use_soft_vol).toBool();
	softvol_max = set->value("softvol_max", softvol_max).toInt();
	use_hwac3 = set->value("use_hwac3", use_hwac3 ).toBool();
//	use_audio_equalizer = set->value("use_audio_equalizer", use_audio_equalizer ).toBool();

	global_volume = set->value("global_volume", global_volume).toBool();
	volume = set->value("volume", volume).toInt();
	mute = set->value("mute", mute).toBool();

	use_mc = set->value("use_mc", use_mc).toBool();
	mc_value = set->value("mc_value", mc_value).toDouble();

	osd = set->value("osd", osd).toInt();
	osd_delay = set->value("osd_delay", osd_delay).toInt();

	file_settings_method = set->value("file_settings_method", file_settings_method).toString();

        play_only_this = set->value("play_only_this", play_only_this).toBool();

	set->endGroup(); // general


    /* ***************
       Drives (CD/DVD)
       *************** */
/*
	set->beginGroup( "drives");

	dvd_device = set->value("dvd_device", dvd_device).toString();
	cdrom_device = set->value("cdrom_device", cdrom_device).toString();

#ifdef Q_OS_WIN
	enable_audiocd_on_windows = set->value("enable_audiocd_on_windows", enable_audiocd_on_windows).toBool();
#endif

	vcd_initial_title = set->value("vcd_initial_title", vcd_initial_title ).toInt();

#if DVDNAV_SUPPORT
	use_dvdnav = set->value("use_dvdnav", use_dvdnav).toBool();
#endif

	set->endGroup(); // drives
*/

    /* ***********
       Performance
       *********** */

	set->beginGroup( "performance");

	priority = set->value("priority", priority).toInt();
	threads = set->value("threads", threads).toInt();

	cache_for_files = set->value("cache_for_files", cache_for_files).toInt();
	cache_for_streams = set->value("cache_for_streams", cache_for_streams).toInt();
/*	cache_for_dvds = set->value("cache_for_dvds", cache_for_dvds).toInt();
	cache_for_vcds = set->value("cache_for_vcds", cache_for_vcds).toInt();
	cache_for_audiocds = set->value("cache_for_audiocds", cache_for_audiocds).toInt();
	cache_for_tv = set->value("cache_for_tv", cache_for_tv).toInt();
*/
	set->endGroup(); // performance



    /* ********
       Advanced
       ******** */

	set->beginGroup( "advanced");

#if USE_COLORKEY
	bool ok;
	QString color = set->value("color_key", QString::number(color_key,16)).toString();
	unsigned int temp_color_key = color.toUInt(&ok, 16);
	if (ok)
		color_key = temp_color_key;
	//color_key = set->value("color_key", color_key).toInt();
#endif

//	use_idx = set->value("use_idx", use_idx).toBool();

	mplayer_additional_options = set->value("mplayer_additional_options", mplayer_additional_options).toString();
	mplayer_additional_audio_filters = set->value("mplayer_additional_audio_filters", mplayer_additional_audio_filters).toString();

	log_mplayer = set->value("log_mplayer", log_mplayer).toBool();
        log_amplayer = set->value("log_amplayer", log_amplayer).toBool();
	log_filter = set->value("log_filter", log_filter).toString();
	verbose_log = set->value("verbose_log", verbose_log).toBool();
        save_amplayer_log = set->value("save_amplayer_log", save_amplayer_log).toBool();

    //mplayer log autosaving
    autosave_mplayer_log = set->value("autosave_mplayer_log", autosave_mplayer_log).toBool();
    mplayer_log_saveto = set->value("mplayer_log_saveto", mplayer_log_saveto).toString();
    //mplayer log autosaving end


//	use_edl_files = set->value("use_edl_files", use_edl_files).toBool();

	prefer_ipv4 = set->value("prefer_ipv4", prefer_ipv4).toBool();

//	use_short_pathnames = set->value("use_short_pathnames", use_short_pathnames).toBool();

//	use_pausing_keep_force = set->value("use_pausing_keep_force", use_pausing_keep_force).toBool();

//	use_correct_pts = (OptionState) set->value("correct_pts", use_correct_pts).toInt();

	actions_to_run = set->value("actions_to_run", actions_to_run).toString();

	set->endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

	set->beginGroup("gui");

	compact_mode = set->value("compact_mode", compact_mode).toBool();
	stay_on_top = (Preferences::OnTop) set->value("stay_on_top", (int) stay_on_top).toInt();
	size_factor = set->value("size_factor", size_factor).toInt();
	resize_method = set->value("resize_method", resize_method).toInt();

#if STYLE_SWITCHING
	style = set->value("style", style).toString();
#endif

	mouse_left_click_function = set->value("mouse_left_click_function", mouse_left_click_function).toString();
	mouse_right_click_function = set->value("mouse_right_click_function", mouse_right_click_function).toString();
	mouse_double_click_function = set->value("mouse_double_click_function", mouse_double_click_function).toString();
	mouse_middle_click_function = set->value("mouse_middle_click_function", mouse_middle_click_function).toString();
	mouse_xbutton1_click_function = set->value("mouse_xbutton1_click_function", mouse_xbutton1_click_function).toString();
	mouse_xbutton2_click_function = set->value("mouse_xbutton2_click_function", mouse_xbutton2_click_function).toString();
	wheel_function = set->value("mouse_wheel_function", wheel_function).toInt();
	int wheel_function_cycle_int = set->value("wheel_function_cycle", (int) wheel_function_cycle).toInt();
	wheel_function_cycle = QFlags<Preferences::WheelFunctions> (QFlag(wheel_function_cycle_int));
	wheel_function_seeking_reverse = set->value("wheel_function_seeking_reverse", wheel_function_seeking_reverse).toBool();

	seeking1 = set->value("seeking1", seeking1).toInt();
	seeking2 = set->value("seeking2", seeking2).toInt();
	seeking3 = set->value("seeking3", seeking3).toInt();
	seeking4 = set->value("seeking4", seeking4).toInt();

//	update_while_seeking = set->value("update_while_seeking", update_while_seeking).toBool();
#if ENABLE_DELAYED_DRAGGING
	time_slider_drag_delay = set->value("time_slider_drag_delay", time_slider_drag_delay).toInt();
#endif
#if SEEKBAR_RESOLUTION
	relative_seeking = set->value("relative_seeking", relative_seeking).toBool();
#endif

	language = set->value("language", language).toString();
	iconset= set->value("iconset", iconset).toString();

	balloon_count = set->value("balloon_count", balloon_count).toInt();

	save_window_size_on_exit = 	set->value("save_window_size_on_exit", save_window_size_on_exit).toBool();

	close_on_finish = set->value("close_on_finish", close_on_finish).toBool();

	default_font = set->value("default_font", default_font).toString();

//	pause_when_hidden = set->value("pause_when_hidden", pause_when_hidden).toBool();

	gui = set->value("gui", gui).toString();

#if USE_MINIMUMSIZE
	gui_minimum_width = set->value("gui_minimum_width", gui_minimum_width).toInt();
#endif
	default_size = set->value("default_size", default_size).toSize();

//	report_mplayer_crashes = set->value("report_mplayer_crashes", report_mplayer_crashes).toBool();

#if REPORT_OLD_MPLAYER
	reported_mplayer_is_old = set->value("reported_mplayer_is_old", reported_mplayer_is_old).toBool();
#endif

	auto_add_to_playlist = set->value("auto_add_to_playlist", auto_add_to_playlist).toBool();
	add_to_playlist_consecutive_files = set->value("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files).toBool();


        status_bar = set->value("status_bar", status_bar).toBool();
        status_bar_format = set->value("status_bar_format", status_bar_format).toString();
        window_title_format = set->value("window_title_format", window_title_format).toString();

        main_stylesheet = set->value("main_stylesheet", main_stylesheet).toString();


        pl_alternate_colors = set->value("pl_alternate_colors", pl_alternate_colors).toBool();
        pl_custom_colors = set->value("pl_custom_colors", pl_custom_colors).toBool();


        pl_columns_names = set->value("pl_columns_names", pl_columns_names).toStringList();
        pl_columns_format = set->value("pl_columns_format", pl_columns_format).toStringList();
        pl_columns_back = set->value("pl_columns_back", pl_columns_back).toStringList();
        pl_columns_playing_format = set->value("pl_columns_playing_format", pl_columns_playing_format).toStringList();
        pl_columns_playng_back = set->value("pl_columns_playng_back", pl_columns_playng_back).toStringList();
        pl_columns_sizes = set->value("pl_columns_sizes", pl_columns_sizes).toStringList();
        pl_columns_aligment = set->value("pl_columns_aligment", pl_columns_aligment).toStringList();

        pl_art_search_pattern = set->value("pl_art_search_pattern", pl_art_search_pattern).toStringList();

        pl_color_text = set->value("pl_color_text", pl_color_text).toStringList();
        pl_color_back = set->value("pl_color_back", pl_color_back).toStringList();
        pl_color_play_text = set->value("pl_color_play_text", pl_color_play_text).toStringList();
        pl_color_play_back = set->value("pl_color_play_back", pl_color_play_back).toStringList();

        pl_stylesheet = set->value("pl_stylesheet", pl_stylesheet).toString();

        pl_groups_labels = set->value("pl_groups_labels", pl_groups_labels).toBool();
        pl_groups_format = set->value("pl_groups_format", pl_groups_format).toString();
        pl_groups_stylesheet = set->value("pl_groups_stylesheet", pl_groups_stylesheet).toString();
        pl_groups_text_color = set->value("pl_groups_text_color", pl_groups_text_color).toString();
        pl_groups_back_color = set->value("pl_groups_back_color", pl_groups_back_color).toString();
        pl_groups_aligment = set->value("pl_groups_aligment", pl_groups_aligment).toInt();
        pl_group_height = set->value("pl_group_height", pl_group_height).toInt();


        pl_row_height = set->value("pl_row_height", pl_row_height).toInt();

        pl_show_playing_time = set->value("pl_show_playing_time", pl_show_playing_time).toBool();
        pl_use_html = set->value("pl_use_html", pl_use_html).toBool();
        pl_use_groups = set->value("pl_use_groups", pl_use_groups).toBool();
        pl_auto_scroll = set->value("pl_auto_scroll", pl_auto_scroll).toBool();


        res_main_width = set->value("res_main_width", res_main_width).toInt();
        res_main_height = set->value("res_main_height", res_main_height).toInt();

        res_pref_width = set->value("res_pref_width", res_pref_width).toInt();
        res_pref_height = set->value("res_pref_height", res_pref_height).toInt();

        res_tree_width = set->value("res_tree_width", res_tree_width).toInt();

        recursive_dirs = set->value("recursive_dirs", recursive_dirs).toBool();
        //QStringList res_lst = set->value("window_pos", res_lst).toString().split("x");
        //x = QString(res_lst.at(0)).toInt();
        //y = QString(res_lst.at(1)).toInt();

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
//	initial_audio_equalizer = set->value("initial_audio_equalizer", initial_audio_equalizer).toList();
        initial_volnorm = set->value("initial_volnorm", initial_volnorm).toBool();

	initial_audio_channels = set->value("initial_audio_channels", initial_audio_channels).toInt();
	initial_stereo_mode = set->value("initial_stereo_mode", initial_stereo_mode).toInt();

	initial_audio_track = set->value("initial_audio_track", initial_audio_track).toInt();

	set->endGroup(); // defaults


    /* ************
       MPlayer info
       ************ */

	set->beginGroup( "mplayer_info");
	mplayer_detected_version = set->value("mplayer_detected_version", mplayer_detected_version).toInt();
	mplayer_user_supplied_version = set->value("mplayer_user_supplied_version", mplayer_user_supplied_version).toInt();
	set->endGroup(); // mplayer_info


    /* *********
       Instances
       ********* */

	set->beginGroup("instances");
	use_single_instance = set->value("use_single_instance", use_single_instance).toBool();
	connection_port = set->value("connection_port", connection_port).toInt();
	use_autoport = set->value("use_autoport", use_autoport).toBool();
	autoport = set->value("temp/autoport", autoport).toInt();
	set->endGroup(); // instances


    /* ****************
       Floating control
       **************** */

	set->beginGroup("floating_control");
	floating_control_margin = set->value("margin", floating_control_margin).toInt();
        floating_control_width = set->value("width", floating_control_width).toInt();
	floating_control_animated = set->value("animated", floating_control_animated).toBool();
	floating_display_in_compact_mode = set->value("display_in_compact_mode", floating_display_in_compact_mode).toBool();
#ifndef Q_OS_WIN
	bypass_window_manager = set->value("bypass_window_manager", bypass_window_manager).toBool();
#endif
	set->endGroup(); // floating_control

}

#endif // NO_USE_INI_FILES
