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
#include <QApplication>

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

    log = true;
    save_log = true;
    log_filter = ".*";


    stay_on_top = false;

    language = "";
    iconset = "";

    close_on_finish = false;

    auto_add_to_playlist = true;
    add_to_playlist_consecutive_files = false;


    res_main_width = 1080;
    res_main_height = 576;

    x=100;
    y=100;


    music_library_path = QDir::homePath();
    files_filter = QString("*.mp3;*.wv;*.flac;*.ogg;*.dts;*.ape;*.m4a;*.mp4;*.ac3;*.wma");

    translations_path = config_path + QDir::separator() + "translations";

}

#ifndef NO_USE_INI_FILES
void Preferences::save() {
        //qDebug("Preferences::save");

#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, "", QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#endif
        config_path = QFileInfo(set.fileName()).absolutePath();
        translations_path = config_path + QDir::separator() + "translations";
        set.setIniCodec(QTextCodec::codecForLocale());



    /* *******
       General
       ******* */

        set.beginGroup( "general");

        set.setValue("driver/audio_output", ao);
        set.setValue("volume", volume);
        set.setValue("mute", mute);

        set.endGroup(); // general



    /* ********
       Advanced
       ******** */

        set.beginGroup( "advanced");

        set.setValue("log", log);
        set.setValue("log_filter", log_filter);
        set.setValue("save_log", save_log);

        set.endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

        set.beginGroup("gui");

        set.setValue("stay_on_top", (int) stay_on_top);
        set.setValue("language", language);
        set.setValue("iconset", iconset);

        set.setValue("close_on_finish", close_on_finish);

        set.setValue("auto_add_to_playlist", auto_add_to_playlist);
        set.setValue("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files);


        set.setValue("res_main_width", res_main_width);
        set.setValue("res_main_height", res_main_height);

        set.setValue("window_x", x);
        set.setValue("window_y", y);


        set.endGroup(); // gui


    /* ***********
       Directories
       *********** */

        set.beginGroup( "directories");
        set.setValue("music_library_path", music_library_path);
        set.setValue("files_filter", files_filter);
        set.setValue("translations_path", translations_path);
        set.endGroup(); // directories

        set.sync();
}



void Preferences::load() {
    //    qDebug("Preferences::load");


#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, "", QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#endif

        config_path = QFileInfo(set.fileName()).absolutePath();
        translations_path = config_path + QDir::separator() + "translations";
        set.setIniCodec(QTextCodec::codecForLocale());



    /* *******
       General
       ******* */

        set.beginGroup( "general");

        ao = set.value("driver/audio_output", ao).toString();
        volume = set.value("volume", volume).toInt();
        mute = set.value("mute", mute).toBool();

        set.endGroup(); // general



    /* ********
       Advanced
       ******** */

        set.beginGroup( "advanced");

        log = set.value("log", log).toBool();
        log_filter = set.value("log_filter", log_filter).toString();
        save_log = set.value("save_log", save_log).toBool();

        set.endGroup(); // advanced


    /* *********
       GUI stuff
       ********* */

        set.beginGroup("gui");

        stay_on_top = set.value("stay_on_top", stay_on_top).toBool();

        language = set.value("language", language).toString();
        iconset= set.value("iconset", iconset).toString();

        close_on_finish = set.value("close_on_finish", close_on_finish).toBool();

        auto_add_to_playlist = set.value("auto_add_to_playlist", auto_add_to_playlist).toBool();
        add_to_playlist_consecutive_files = set.value("add_to_playlist_consecutive_files", add_to_playlist_consecutive_files).toBool();

        res_main_width = set.value("res_main_width", res_main_width).toInt();
        res_main_height = set.value("res_main_height", res_main_height).toInt();

        x = set.value("window_x", x).toInt();
        y = set.value("window_y", y).toInt();

        set.endGroup(); // gui


    /* ***********
       Directories
       *********** */

        set.beginGroup( "directories");

        music_library_path = set.value("music_library_path", music_library_path).toString();
        files_filter = set.value("files_filter", files_filter).toString();
        translations_path = set.value("translations_path", translations_path).toString();
        set.endGroup(); // directories

}

#endif // NO_USE_INI_FILES
