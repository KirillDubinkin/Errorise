#include "albumtree.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QList>
#include <QRegExp>
#include <QMapIterator>
#include <QMap>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDir>

AlbumTree::AlbumTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->header()->setVisible(false);
    //ptrn = "%filepath%/%artist%/[%date%] %album%";
    ptrn = "%filepath%";

    if (mlib->isReady())
        fillTree();

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(selectedNodeChange(QTreeWidgetItem*)));
}


static QRegExp rx_tag("%([a-z]*)%");



void AlbumTree::selectedNodeChange(QTreeWidgetItem *cur)
{
    QStringList list;
    list.append(cur->text(0));
    while (cur->parent())
    {
        cur = cur->parent();
        list.insert(0, cur->text(0));
    }
    QString s = list.join(QDir::separator());
    s.insert(0, mlib->libraryPath());

    mlib->selectTracksBy(getTags(ptrn).at(0), s);
}



void AlbumTree::fillTree()
{
    QMap<QString, int> map = firstNode();

   // if (ptrn.contains("filepath"))
   // {
        mkFsTree(map);
   // }

}

/*
void AlbumTree::addNode(QList<QStringList> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        QStringList srt  = list.at(i);
        QStringList prev = list.at(i-1);

        for (int j = 0; j < prev.size(); j++)
        {

        }
    }
}


void AlbumTree::addNode(QList<QStringList> list)
{
    QList<QStringList> treeList;
    int iTree = 1;

    treeList.append(list.first());

    for (int l = 1; l < list.size(); l++)
    {
        QStringList str = list.at(l);
        QStringList prev = list.at(l-1);

        for (int i = 0; i < str.size(); i++)
        {
            if (i < prev.size())
            {
                if (str.at(i) != prev.at(i))

            }
        }
    }
}
*/

void AlbumTree::mkFsTree(const QMap<QString, int> &map)
{
    QMapIterator<QString, int> i(map);

    QList<QTreeWidgetItem *> list;

    while (i.hasNext())
    {
        i.next();
        QString s = i.key();
        qDebug() << s;

        if (list.size())
        {
            QTreeWidgetItem *item = list.at(list.size()-1);

            while (s.contains(item->text(0)))
            {
                s.remove(item->text(0));
                if (s.at(0) == QDir::separator())
                    s.remove(0, 1);
                if (item->childCount())
                    item = item->child(item->childCount()-1);
                else
                    break;
            }


            if (item->parent())
                item = item->parent();


            while (!s.isEmpty())
            {
                QString t = s.mid(0, s.indexOf(QDir::separator()));
                QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList(t));
                item->addChild(itm);
                item = itm;
                s.remove(0, t.size()+1);
            }

        }
        else
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(s.mid(0, s.indexOf(QDir::separator()))));
            list.append(item);
            s.remove(item->text(0) + QDir::separator());
        }
    }
    this->invisibleRootItem()->addChildren(list);


//    this->addTopLevelItems(list);
}


QMap<QString, int> AlbumTree::firstNode()
{
    QString tag = ptrn.mid(0, ptrn.indexOf("/")).remove("%");

    QStringList tags = getTags(ptrn);
    qDebug() << tags;

    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tags.join(", ") + " FROM tracks"))
    {
        QMap<QString, int> map;

        if (tag.contains("filepath"))
        {
            QString s = mlib->libraryPath() + QDir::separator();

            while (query.next())
                map.insert(query.value(0).toString().remove(s) + QDir::separator(), 0);
            map.remove(mlib->libraryPath());
        }
        else
        {
            while (query.next())
            {
                QString s = ptrn;

                for (int i = 0; i < tags.size(); i++)
                {
                    s.replace(tags.at(i), query.value(i).toString());
                }
                s.remove("%");
                s.append(QDir::separator());

                map.insert(s, 0);
            }
        }

        return map;
    }
    qWarning() << query.lastError();
}


    //! Паттерн - это одна из заданных ветвей дерева
QStringList AlbumTree::getTags(QString pattern)
{
    QStringList list;
    int pos = 0;

    while ((pos = rx_tag.indexIn(pattern, pos)) != -1)
    {
        list.append(rx_tag.cap(1));
        pos += rx_tag.matchedLength();
    }

    return list; // тэги из паттерна одной ветви
}


