#ifndef SIMPLEPLPREFS_H
#define SIMPLEPLPREFS_H


#include <QString>
#include <QStringList>
#include <QList>

class SimplePLPrefs
{
public:
    SimplePLPrefs();
    ~SimplePLPrefs();

    void reset();
    void load();
    void save();

    //! Groups
    QString groups_format;
    QString groups_text_color;
    QString groups_back_color;
    int groups_text_aligment;
    int group_height;
    int group_delay;
    bool group_header;
    bool group_byDirs;


    //! Columns
    QStringList columns_names;
    QList<int> columns_sizes;
    QList<int> columns_aligment;


    //! Rows
    QStringList rows_format;
    QStringList rows_playback_format;

    //! Colors
    QStringList color_column_text;
    QStringList color_column_back;
    QStringList color_playback_text;
    QStringList color_playback_back;


    //! Other
    int row_height;
    QString stylesheet;
    bool alternate_colors;
    bool show_header;
    int  smooth_art_scale;


private:
    QString filename;
    QStringList s_col_size;
    QStringList s_col_align;
};

#endif // SIMPLEPLPREFS_H
