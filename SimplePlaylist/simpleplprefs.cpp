#include "simpleplprefs.h"

#include <QSettings>
#include <QTextCodec>
#include <QApplication>

const QString PlugName = "SimplePlayList";

SimplePLPrefs::SimplePLPrefs()
{
    reset();
    load();
    save();
}

SimplePLPrefs::~SimplePLPrefs()
{
    save();
}


void SimplePLPrefs::reset()
{

    //! Groups
    groups_format        = "[$if(%albumartist%,%artist%) - ]['['%date%']'] %album%";
    groups_text_color    = "ffffff";
    groups_back_color    = "182835";
    groups_text_aligment = Qt::AlignLeft;
    group_height         = 20;
    group_delay          = 2;
    group_header         = true;
    group_byDirs         = true;

    //! Columns
    columns_names      << "Cover" << "#" << "Length" << "Track Name" << "Format";
    columns_sizes      << 300 << 40 << 80 << 200 << 80;
    columns_aligment   << 4 << 4 << 4 << 4 << 4;

    //! Rows
    rows_format              << "%art%" << "%tracknumber%" << "%duration%" << "%title%" << "%format%";
    rows_playback_format     << "%art%" << "%tracknumber%" << "%duration%" << "%title%" << "%format%";

    //! Colors
    color_column_text << "" << "527482" << "43606b" << "" << "527482";
    color_column_back << "" << "" << "" << "" << "";

    //! Other
    stylesheet       = "font: 9pt \"Ubuntu\"; gridline-color: rgb(192, 192, 192); \nbackground-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(9, 14, 17, 255), stop:1 rgba(50, 57, 64, 255)); \ncolor: rgb(200,200,200);\ngridline-color: rgb(74, 91, 108); \nalternate-background-color: rgb(28, 36, 45); \nselection-background-color: rgb(48, 56, 65); \nselection-color: rgb(157, 196, 255);";
    row_height       = 16;
    alternate_colors = false;
    show_header      = false;
    smooth_art_scale = Qt::SmoothTransformation;

}


void SimplePLPrefs::save()
{
#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    set.setValue("groups_format", groups_format);
    set.setValue("groups_text_color", groups_text_color);
    set.setValue("groups_back_color", groups_back_color);
    set.setValue("groups_text_aligment", groups_text_aligment);
    set.setValue("group_height", group_height);
    set.setValue("group_delay", group_delay);
    set.setValue("group_header", group_header);
    set.setValue("group_byDirs", group_byDirs);
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    set.setValue("columns_names", columns_names);

    s_col_size.clear();
    for (int i = 0; i < columns_sizes.size(); i++)
        s_col_size << QString().number(columns_sizes.at(i));
    set.setValue("columns_sizes", s_col_size);

    s_col_align.clear();
    for (int i = 0; i < columns_aligment.size(); i++)
        s_col_align << QString().number(columns_aligment.at(i));
    set.setValue("columns_aligment", s_col_align);
    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    set.setValue("rows_format", rows_format);
    set.setValue("rows_playback_format", rows_playback_format);
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    set.setValue("color_column_text", color_column_text);
    set.setValue("color_column_back", color_column_back);
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    set.setValue("stylesheet", stylesheet);
    set.setValue("row_height", row_height);
    set.setValue("alternate_colors", alternate_colors);
    set.setValue("show_header", show_header);
    set.setValue("smooth_art_scale", smooth_art_scale);
    set.endGroup();


    set.sync();
}


void SimplePLPrefs::load()
{
#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    groups_format = set.value("groups_format", groups_format).toString();
    groups_text_color = set.value("groups_text_color", groups_text_color).toString();
    groups_back_color = set.value("groups_back_color", groups_back_color).toString();
    groups_text_aligment = set.value("groups_text_aligment", groups_text_aligment).toInt();
    group_delay          = set.value("group_delay", group_delay).toInt();
    group_height         = set.value("group_height", group_height).toInt();
    group_header = set.value("group_header", group_header).toBool();
    group_byDirs = set.value("group_byDirs", group_byDirs).toBool();
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    columns_names = set.value("columns_names", columns_names).toStringList();

    s_col_size = set.value("columns_sizes", s_col_size).toStringList();
    if (!s_col_size.isEmpty()) {
        columns_sizes.clear();
        for (int i = 0; i < s_col_size.size(); i++)
            columns_sizes << QString(s_col_size.at(i)).toInt();
    }

    s_col_align = set.value("columns_aligment", s_col_align).toStringList();
    if (!s_col_align.isEmpty()) {
        columns_aligment.clear();
        for (int i = 0; i < s_col_align.size(); i++)
            columns_aligment << QString(s_col_align.at(i)).toInt();
    }

    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    rows_format = set.value("rows_format", rows_format).toStringList();
    rows_playback_format = set.value("rows_playback_format", rows_playback_format).toStringList();
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    color_column_text = set.value("color_column_text", color_column_text).toStringList();
    color_column_back = set.value("color_column_back", color_column_back).toStringList();
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    stylesheet       = set.value("stylesheet", stylesheet).toString();
    row_height       = set.value("row_height", row_height).toInt();
    alternate_colors = set.value("alternate_colors", alternate_colors).toBool();
    show_header      = set.value("show_header", show_header).toBool();
    smooth_art_scale = set.value("smooth_art_scale", smooth_art_scale).toInt();
    set.endGroup();
}

