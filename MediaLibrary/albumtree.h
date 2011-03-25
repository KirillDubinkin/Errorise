#ifndef ALBUMTREE_H
#define ALBUMTREE_H

#include <QTreeWidget>
#include <QStringList>
#include <QMultiMap>
#include <QModelIndex>

#include "MediaLibrary/albumtreeprefs.h"
#include "global.h"

using namespace Global;

class AlbumTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AlbumTree(QWidget *parent = 0);
    ~AlbumTree();

signals:
   // void selectedTracksChanged(QString tag, QString value);

public slots:

private slots:
    void fillTree();
    void selectedNodeChange(QTreeWidgetItem *cur);

private:
    QMap<QString, int> firstNode();
    void mkTree(const QMap<QString, int> &map);

private:
    QString ptrn;
    QMultiMap<QString, int> tracks;
    AlbumTreePrefs *prefs;


};

#endif // ALBUMTREE_H
