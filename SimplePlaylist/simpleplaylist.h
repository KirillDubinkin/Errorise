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

    QList<int> getGroup(const QList<int> &tracks);
    QString doPlaylistArt(QString filename, QString dir);

    void loadSettings();
    void createActions();

private slots:
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
    void setColumnWidth(int column, int width);
    void setColumnAlign(int column, int align);
};

#endif // SIMPLEPLAYLIST_H
