#ifndef SIMPLEALBUMLIST_H
#define SIMPLEALBUMLIST_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QString>

#include "global.h"

using namespace Global;

class SimpleAlbumlist : public QTreeView
{
    Q_OBJECT
public:
    explicit SimpleAlbumlist(QWidget *parent = 0);

signals:
    void directoryChanged(QString path);

public slots:
    void setPath(QString path = pref->music_library_path);

private:
    QFileSystemModel * FSmodel;

private slots:
    void pathChanged(const QModelIndex &, const QModelIndex &);

};

#endif // SIMPLEALBUMLIST_H
