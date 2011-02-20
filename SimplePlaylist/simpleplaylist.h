#ifndef SIMPLEPLAYLIST_H
#define SIMPLEPLAYLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDir>
#include <QStringList>
#include <QList>

#include "SimplePlaylist/simpleplprefs.h"

#include "helper.h"
#include "global.h"
#include "interfaces/playerinterface.h"
#include "interfaces/queueinterface.h"

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
    void setTracks(const QList<int> &GUID);
    void setTracksWithGroups(const QList<int> &GUID);


    void highlightCurrentTrack();
    void defPlhighlight();

private:
    SimplePLPrefs * prefs;
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
