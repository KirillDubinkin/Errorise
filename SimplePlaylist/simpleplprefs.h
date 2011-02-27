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
    int groups_text_aligment;
    QString groups_back_color;
    QString groups_stylesheet;
    int group_height;
    bool group_labels;


    //! Columns
    QStringList columns_names;
    QList<int> columns_sizes;
    QList<int> columns_aligment;
    QStringList columns_stylesheet;


    //! Rows
    QStringList rows_format;
    QStringList rows_stylesheet;
    QStringList rows_playback_format;
    QStringList rows_playback_stylesheet;
    bool labels;

    //! Colors
    QStringList color_column_text;
    QStringList color_column_back;
    QStringList color_playback_text;
    QStringList color_playback_back;


    //! Other
    int row_height;
    QStringList art_search_pattern;
    QStringList art_search_folders;
    QString stylesheet;
    bool alternate_colors;
    bool show_header;


private:
    QString filename;
    QStringList s_col_size;
    QStringList s_col_align;
};

#endif // SIMPLEPLPREFS_H
