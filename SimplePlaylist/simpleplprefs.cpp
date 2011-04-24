#include "simpleplprefs.h"

#include <QSettings>
#include <QTextCodec>
#include <QApplication>

const QString PlugName = "SimplePlayList";

SimplePLPrefs::SimplePLPrefs()
{

    reset();
    load();
}

SimplePLPrefs::~SimplePLPrefs()
{
    save();
}


void SimplePLPrefs::reset()
{
    //qDebug("SimplePlaylist->Prefs::reset()");

    //! Groups
    groups_format = "[%date%] %album%";
    groups_text_color = "ffffff";
    groups_text_aligment = Qt::AlignLeft;
    groups_back_color = "13363b";
    groups_stylesheet = "background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.681, fx:0.5, fy:0.5, stop:0 rgba(0, 35, 51, 255), stop:1 rgba(0, 74, 92, 255));\ncolor: rgb(255, 255, 255);";
    group_height = 20;
    group_labels = true;

    //! Columns
    columns_names << "Cover" << "#" << "Length" << "Track Name" << "Format";
    columns_sizes << 300 << 40 << 80 << 200 << 80;
    columns_aligment << 4 << 4 << 4 << 4 << 4;
    columns_stylesheet << "" << "" << "" << "" << "";

    //! Rows
    rows_format << "%art%" << "%tracknumber%" << "%duration%" << "%title%" << "%format%";
    rows_stylesheet << "" << "" << "" << "" << "";
    rows_playback_format << "%art%" << "%tracknumber%" << "%duration%" << "%title%" << "%format%";
    rows_playback_stylesheet << "" << "" << "" << "" << "";
    labels = false;

    //! Colors
    color_column_text << "" << "527482" << "43606b" << "" << "527482";
    color_column_back << "" << "" << "" << "" << "";

    //! Other
    art_search_pattern << "*cover*.jpg" << "*folder*.jpg" << "*front*.jpg";
    art_search_folders << "art" << "artwork" << "covers";
    stylesheet = "font: 9pt \"Ubuntu\"";
    row_height = 16;
    alternate_colors = true;
    show_header = false;


}


void SimplePLPrefs::save()
{
    //qDebug("SimplePlaylist->Prefs::save()");

#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, "", QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    set.setValue("groups_format", groups_format);
    set.setValue("groups_text_color", groups_text_color);
    set.setValue("groups_back_color", groups_back_color);
    set.setValue("groups_text_aligment", groups_text_aligment);
    set.setValue("groups_stylesheet", groups_stylesheet);
    set.setValue("group_height", group_height);
    set.setValue("group_labels", group_labels);
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

    set.setValue("columns_stylesheet", columns_stylesheet);
    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    set.setValue("rows_format", rows_format);
    set.setValue("rows_stylesheet", rows_stylesheet);
    set.setValue("rows_playback_format", rows_playback_format);
    set.setValue("rows_playback_stylesheet", rows_playback_stylesheet);
    set.setValue("labels", labels);
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    set.setValue("color_column_text", color_column_text);
    set.setValue("color_column_back", color_column_back);
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    set.setValue("art_search_pattern", art_search_pattern);
    set.setValue("art_search_folders", art_search_folders);
    set.setValue("stylesheet", stylesheet);
    set.setValue("row_height", row_height);
    set.setValue("alternate_colors", alternate_colors);
    set.setValue("show_header", show_header);

    set.endGroup();


    set.sync();
}


void SimplePLPrefs::load()
{
    //qDebug("SimplePlaylist->Prefs::load()");

#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, "", QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    groups_format = set.value("groups_format", groups_format).toString();
    groups_text_color = set.value("groups_text_color", groups_text_color).toString();
    groups_back_color = set.value("groups_back_color", groups_back_color).toString();
    groups_text_aligment = set.value("groups_text_aligment", groups_text_aligment).toInt();
    groups_stylesheet = set.value("groups_stylesheet", groups_stylesheet).toString();
    group_height = set.value("group_height", group_height).toInt();
    group_labels = set.value("group_labels", group_labels).toBool();
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

    columns_stylesheet = set.value("columns_stylesheet", columns_stylesheet).toStringList();
    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    rows_format = set.value("rows_format", rows_format).toStringList();
    rows_stylesheet = set.value("rows_stylesheet", rows_stylesheet).toStringList();
    rows_playback_format = set.value("rows_playback_format", rows_playback_format).toStringList();
    rows_playback_stylesheet = set.value("rows_playback_stylesheet", rows_playback_stylesheet).toStringList();
    labels = set.value("labels", labels).toBool();
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    color_column_text = set.value("color_column_text", color_column_text).toStringList();
    color_column_back = set.value("color_column_back", color_column_back).toStringList();
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    art_search_pattern = set.value("art_search_pattern", art_search_pattern).toStringList();
    art_search_folders = set.value("art_search_folders", art_search_folders).toStringList();
    stylesheet = set.value("stylesheet", stylesheet).toString();
    row_height = set.value("row_height", row_height).toInt();
    alternate_colors = set.value("alternate_colors", alternate_colors).toBool();
    show_header = set.value("show_header", show_header).toBool();
    set.endGroup();
}

