#ifndef SIMPLEPLAYLIST_H
#define SIMPLEPLAYLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QStringList>
#include <QList>

#include "SimplePlaylist/simpleplprefs.h"
#include "SimplePlaylist/simpleplaylistprefswidget.h"
#include "SimplePlaylist/coversqueue.h"

class SimplePlaylist : public QTableWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylist(QWidget *parent = 0);
    ~SimplePlaylist();

    QWidget *getPrefsWidget();

signals:
    void needPrefWindow();

public slots:    
    void setColumns();
    void fillPlaylist();

    void highlightCurrentTrack(QString filename, int guid);
    void highlightCurrentTrack();
    void defPlhighlight();

private:
    SimplePLPrefs * prefs;
    SimplePlaylistPrefsWidget  *prefsWidget;
    QList<int> trackGuids;
    QList<CoversQueue> artQueue;

    QTableWidgetItem *newItem(Qt::ItemFlags flags = Qt::NoItemFlags, const QString &text = "");

    void addRowItem(int row, int col, const QString &text);
    void addGroupItem(int row, const QString &text);

    int CoverColumn;
    int LengthColumn;
    int currentTrackRow;

    bool findCurrentTrack(int guid);
    bool findCurrentTrack(QString filename);

    QList<int> getSelectedIds();

    QString doPlaylistArt(QString filename, QString dir);

    void loadSettings();

private slots:
    void createActions();

    void play(int row);
    void playNext();
    bool addNextTrack();
    void finished();

    void addToQueue();

    void setAlternatingRowColors(bool enable) { QTableWidget::setAlternatingRowColors(enable); }

    void getNewTracks(QStringList tags, QStringList values);
    void insertCover();
    void insertLastCovers();
    void deletePreferences() { prefsWidget = 0; }

    void setHeaderVisible(bool enable);
    void setGridStyle(Qt::PenStyle style) { QTableWidget::setGridStyle(style); }

    void setColumnWidth(int column, int width) { QTableWidget::setColumnWidth(column + 1, width); }
    // This slot connected to prefsWidget signal, that sending column as index of colum_names list
    // but, there, first column - hidden, so

    void setColumnAlign(int column, int align);
    void setColumnTextColor(int column, QColor color);
    void setColumnName(int column, QString name);
    void moveColumn(int from, int to);
    void insertNewColumn(int col);

    void setRowsHeight(int height);

    void setGroupRowVisible(bool visible);
    void setGroupRowHeight(int height);
    void setGroupTextAlign(int align);
    void setGroupTextColor(QColor color);
    void setGroupBackColor(QColor color);

    void removeFiles();
    void deleteTracks() { removeTracks(getSelectedIds()); }
    void removeTracks(QList<int> ids);
    void moveFiles();

    void copyFilesTo() {}
    void copyFilesToClipboard() {}
    void renameFiles() {}

    void copyPatternToClipboard();
    void copyPlLineToClipboard();
    void copyCurColTextToClipboard();
};

#endif // SIMPLEPLAYLIST_H
