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

#include "helper.h"

AlbumTree::AlbumTree(QWidget *parent) :
    QTreeWidget(parent)
{
    prefs = new AlbumTreePrefs();

    setStyleSheet(prefs->stylesheet);
    header()->setVisible(prefs->showHeader);

    connect(mlib, SIGNAL(readyToWork()), this, SLOT(fillTree()));

    if (mlib->isReady())
        fillTree();

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(selectedNodeChange(QTreeWidgetItem*)));
}


AlbumTree::~AlbumTree()
{
    delete prefs;
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
    s.insert(0, mlib->libraryPath() + QDir::separator());

    mlib->selectTracksBy(Helper::getTags(prefs->pattern).at(0), s);
}


void AlbumTree::fillTree()
{
    QMap<QString, int> map = firstNode();
    this->mkTree(map);
}



void AlbumTree::mkTree(const QMap<QString, int> &map)
{
    QMapIterator<QString, int> i(map);
    QList<QTreeWidgetItem *> list;

    QTreeWidgetItem *item;

    i.next();
    QString key = i.key();

    item = new QTreeWidgetItem(QStringList(key.mid(0, key.indexOf(QDir::separator()))));
    item->setIcon(0, QIcon(prefs->items_icon));
    list.append(item);
    key.remove(0, item->text(0).size() + 1);

    while (!key.isEmpty())
    {
        QString temp = key.mid(0, key.indexOf(QDir::separator()));
        QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList(temp));
        itm->setIcon(0, QIcon(prefs->items_icon));
        item->addChild(itm);
        item = itm;
        key.remove(0, temp.size() + 1);
    }


    while (i.hasNext())
    {
        i.next();
        key = i.key();
        item = list.at(list.size() - 1);

        if (key.mid(0, key.indexOf(QDir::separator())).contains(item->text(0)))
        {
            while (key.mid(0, key.indexOf(QDir::separator())).contains(item->text(0)))
            {
              //  key.remove(0, item->text(0).size() + 1);
                key.remove(0, item->text(0).size());
                if (key.at(0) == QDir::separator())
                    key.remove(0, 1);

                if (item->childCount())
                    item = item->child(item->childCount() - 1);
                else
                    break;
            }

            if (item->parent())
                item = item->parent();
        }
        else
        {
            item = new QTreeWidgetItem(QStringList(key.mid(0, key.indexOf(QDir::separator()))));
            item->setIcon(0, QIcon(prefs->items_icon));
            list.append(item);
            key.remove(0, item->text(0).size() + 1);
        }


        while (!key.isEmpty())
        {
            QString temp = key.mid(0, key.indexOf(QDir::separator()));
            QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList(temp));
            itm->setIcon(0, QIcon(prefs->items_icon));
            item->addChild(itm);
            item = itm;
            key.remove(0, temp.size() + 1);
        }
    }

    this->invisibleRootItem()->addChildren(list);
}



QMap<QString, int> AlbumTree::firstNode()
{
    QString tag = prefs->pattern;

    QStringList tags = Helper::getTags(prefs->pattern);
    //qDebug() << tags;

    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tags.join(", ") + " FROM tracks "
                   "ORDER BY " + tags.join(", ")))
    {
        QMap<QString, int> map;

        if (tag.contains("filepath"))
        {
            QString s = mlib->libraryPath() + QDir::separator();

            while (query.next())
                map.insert(query.value(0).toString().remove(s) + QDir::separator(), 0);
            map.remove(mlib->libraryPath() + QDir::separator());
        }
        else
        {
            while (query.next())
            {
                QString s = prefs->pattern;

                for (int i = 0; i < tags.size(); i++)
                {
                    if (!query.value(i).toString().isEmpty())
                        s.replace(tags.at(i), query.value(i).toString());
                    else
                        s.replace(tags.at(i), tr(QString("Unknown " + tags.at(i)).toUtf8().data()));
                }
                s.remove("%");
                s.append(QDir::separator());

                map.insert(s, 0);
            }
        }

        return map;
    }
    qWarning() << query.lastError();

    QMap<QString, int> map;
    return map;
}

