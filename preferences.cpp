#include "preferences.h"
#include "global.h"

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

        //! General
    log                  = true;
    save_log             = true;
    log_filter           = ".*";
    close_on_finish      = false;
    auto_add_to_playlist = true; //! Add files to open  playlist


        //! Player
    audio_output = "";
    volume       = 75;
    mute         = false;


        //! GUI
    language          = "";
    iconset           = "";
    translations_path = config_path + "/translations";
    stay_on_top       = false;


        //! Library
    music_library_path  = "";
    files_filter        = "*.mp3;*.wv;*.flac;*.ogg;*.dts;*.ape;*.m4a;*.mp4;*.ac3;*.wma";
    art_search_patterns << "*cover*.jp*g" << "*cover*.png" << "*cover*.gif" << "*cover*.bmp"
                        << "*folder*.jp*g" << "*folder*.png" << "*folder*.gif" << "*folder*.bmp"
                        << "*front*.jp*g"  << "*front*.png" << "*front*.gif" << "*front*.bmp"
                        << "*.jp*g" << "*.png" << "*.gif" << ".bmp";

    art_search_folders  << "art" << "artwork" << "covers" << "Art" << "Artwork" << "ArtWork" << "Covers" << "Images" << "images" << "Scans" << "scans";

}




void Preferences::save()
{

#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#endif
    config_path = QFileInfo(set.fileName()).absolutePath();
    translations_path = config_path + "/translations";
    set.setIniCodec(QTextCodec::codecForLocale());


        //! General
    set.beginGroup("General");
    set.setValue("log", log);
    set.setValue("save_log", save_log);
    set.setValue("log_filter", log_filter);
    set.setValue("close_on_finish", close_on_finish);
    set.setValue("auto_add_to_playlist", auto_add_to_playlist);
    set.endGroup();


        //! Player
    set.beginGroup("Player");
    set.setValue("audio_output", audio_output);
    set.setValue("volume", volume);
    set.setValue("mute", mute);
    set.endGroup();

        //! GUI
    set.beginGroup("Gui");
    set.setValue("language", language);
    set.setValue("iconset", iconset);
    set.setValue("stay_on_top", (int) stay_on_top);
    set.setValue("translations_path", translations_path);
    set.endGroup();

        //! Library
    set.beginGroup("Library");
    set.setValue("music_library_path", music_library_path);
    set.setValue("files_filter", files_filter);
    set.setValue("art_search_patterns", art_search_patterns);
    set.setValue("art_search_folders", art_search_folders);
    set.endGroup();

    set.sync();
}




void Preferences::load()
{

#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), QApplication::applicationName());
#endif

    config_path = QFileInfo(set.fileName()).absolutePath();
    translations_path = config_path + "/translations";
    set.setIniCodec(QTextCodec::codecForLocale());


        //! General
    set.beginGroup("General");
    log                  = set.value("log", log).toBool();
    save_log             = set.value("save_log", save_log).toBool();
    log_filter           = set.value("log_filter", log_filter).toString();
    close_on_finish      = set.value("close_on_finish", close_on_finish).toBool();
    auto_add_to_playlist = set.value("auto_add_to_playlist", auto_add_to_playlist).toBool();
    set.endGroup();


        //! Player
    set.beginGroup("Player");
    audio_output = set.value("audio_output", audio_output).toString();
    volume       = set.value("volume", volume).toInt();
    mute         = set.value("mute", mute).toBool();
    set.endGroup();


        //! GUI
    set.beginGroup("Gui");
    language          = set.value("language", language).toString();
    iconset           = set.value("iconset", iconset).toString();
    stay_on_top       = set.value("stay_on_top", stay_on_top).toBool();
    translations_path = set.value("translations_path", translations_path).toString();
    set.endGroup();


        //! Library
    set.beginGroup("Library");
    music_library_path  = set.value("music_library_path", music_library_path).toString();
    files_filter        = set.value("files_filter", files_filter).toString();
    art_search_patterns = set.value("art_search_patterns", art_search_patterns).toStringList();
    art_search_folders  = set.value("art_search_folders", art_search_folders).toStringList();
    set.endGroup();

}

