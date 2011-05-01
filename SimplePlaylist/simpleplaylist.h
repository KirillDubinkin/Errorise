#ifndef SIMPLEPLAYLIST_H
#define SIMPLEPLAYLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QTime>

#include "SimplePlaylist/simpleplprefs.h"
#include "SimplePlaylist/simpleplaylistprefswindow.h"
#include "SimplePlaylist/coversqueue.h"

#include "helper.h"
#include "global.h"

using namespace Global;

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
    void fillPlaylist();

    void highlightCurrentTrack(QString filename, int guid);
    void highlightCurrentTrack();
    void defPlhighlight();


private:
    SimplePLPrefs * prefs;
    Helper * helper;
    SimplePlaylistPrefsWindow  *prefsWindow;
    QList<int> trackGuids;
    QList<CoversQueue> artQueue;

    QTableWidgetItem *newItem(const QBrush &background, Qt::ItemFlags flags = Qt::ItemIsEnabled,
                              const QString &text = "");

    void addRowItem(int row, int col, const QString &text);
    void addGroupItem(int row, const QString &text);

    int CoverColumn;
    int LengthColumn;
    int currentTrackRow;


    bool findCurrentTrack(int guid);
    bool findCurrentTrack(QString filename);

    QTime time;

    QList<int> getGroup(const QList<int> &tracks);
    void insertCover(int row, const QString &cover);

    void loadSettings();
    void createActions();


private slots:
    void play(int row);
    void playNext();
    bool addNextTrack();
    void finished();

    void showPreferences();
    void addToQueue();

    void setAlternatingRowColors(bool enable) { QTableWidget::setAlternatingRowColors(enable); }

    void getNewTracks(QString tag, QString value);
    void insertCover();
    void insertLastCovers();

};

#endif // SIMPLEPLAYLIST_H
