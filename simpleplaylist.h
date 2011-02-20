#ifndef SIMPLEPLAYLIST_H
#define SIMPLEPLAYLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDir>
#include <QStringList>
#include <QList>

#include "helper.h"
#include "global.h"
#include "interfaces/playerinterface.h"
#include "interfaces/queueinterface.h"

using namespace Global;

class Prefs
{
public:
    Prefs();
    ~Prefs();

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
    bool alternate_colors;
    bool show_header;


private:
    QString filename;
    QStringList s_col_size;
    QStringList s_col_align;
};


class SimplePlaylist : public QTableWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylist(QWidget *parent = 0);
    ~SimplePlaylist();

    int coverColumn();
    int lengthColumn();

signals:

public slots:    
    void setColumns();
    void setTracks(const QList<int> &GUID);
    void setTracksWithGroups(const QList<int> &GUID);


    void highlightCurrentTrack();
    void defPlhighlight();

private:
    Prefs * prefs;
    Helper * helper;
    QTableWidgetItem *newItem(const QBrush &background, Qt::ItemFlags flags = Qt::ItemIsEnabled,
                              const QString &text = "");

    void addRowItem(int row, int col, const QString &text);
    void addRowLabel(int row, int col, const QString &text);

    void addGroupItem(int row, const QString &text);
    void addGroupLabel(int row, const QString &text);

    int addCover(int row, int spanRow, const QString &searchPath);

    int CoverColumn;
    int LengthColumn;


    //PlayerInterface *player;
};

#endif // SIMPLEPLAYLIST_H
