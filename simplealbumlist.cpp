#include "simplealbumlist.h"
#include <QDebug>

SimpleAlbumlist::SimpleAlbumlist(QWidget *parent) :
    QTreeView(parent)
{
    FSmodel = new QFileSystemModel(this);
    FSmodel->setRootPath("");
    FSmodel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    this->setModel(FSmodel);

    this->hideColumn(1);
    this->hideColumn(2);
    this->hideColumn(3);

    setPath();

    connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(pathChanged(QModelIndex,QModelIndex)));
}


void SimpleAlbumlist::pathChanged(const QModelIndex &cur, const QModelIndex &)
{
    emit directoryChanged(FSmodel->filePath(cur));
}

void SimpleAlbumlist::setPath(QString path)
{
    if (!path.isEmpty())
        this->setRootIndex(FSmodel->index(path));
}
