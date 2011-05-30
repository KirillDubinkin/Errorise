#ifndef SIMPLEPLPREFS_H
#define SIMPLEPLPREFS_H


#include <QString>
#include <QStringList>
#include <QList>
#include <QColor>

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
    QColor  groups_text_color;
    QColor  groups_back_color;
    int groups_text_aligment;
    int group_height;
    int group_delay;
    bool group_header;
    bool group_byDirs;


    //! Columns
    QStringList columns_names;
    QList<int>  columns_sizes;
    QList<int>  columns_aligment;
    QStringList rows_format;
    QStringList rows_text_color;


    //! Other
    int row_height;
    QString stylesheet;
    bool alternate_colors;
    bool show_header;

private:
    QString filename;
    QStringList s_col_size;
    QStringList s_col_align;
};

#endif // SIMPLEPLPREFS_H
