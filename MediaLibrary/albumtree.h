#ifndef ALBUMTREE_H
#define ALBUMTREE_H

#include <QTreeWidget>
#include <QStringList>
#include <QMultiMap>
#include <QModelIndex>

#include "global.h"

using namespace Global;

class AlbumTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AlbumTree(QWidget *parent = 0);

signals:
    void selectedTracksChanged(QString tag, QString value);

public slots:

private slots:
    void fillTree();
    void selectedNodeChange(QTreeWidgetItem *cur);

private:
    QStringList getTags(QString pattern);
    QMap<QString, int> firstNode();
    void mkFsTree(const QMap<QString, int> &map);

    void addNode(QList<QStringList> list);

private:
    QString ptrn;
    QMultiMap<QString, int> tracks;


};

#endif // ALBUMTREE_H
