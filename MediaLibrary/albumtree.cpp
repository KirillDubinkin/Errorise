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
#include <QAction>

#include "helper.h"

static QRegExp rx_tag("%([a-z]*)%");

AlbumTree::AlbumTree(QWidget *parent) :
    QTreeWidget(parent)
{
    prefsWidget = 0;
    prefs = new AlbumTreePrefs();

    createMenu();

    setStyleSheet(prefs->stylesheet);
    header()->setVisible(false);
    //setColumnCount(2);
    //hideColumn(1);

    connect(mlib, SIGNAL(readyToWork()), this, SLOT(fillTree()));

    if (mlib->isReady())
        fillTree();

    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(selectedNodeChange(QTreeWidgetItem*)));
}


AlbumTree::~AlbumTree()
{
    delete prefs;
}



void AlbumTree::createMenu()
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *act;

    act = new QAction("Preferences...", this);
    connect(act, SIGNAL(triggered()), this, SIGNAL(needPrefWindow()));
    addAction(act);
}


QWidget * AlbumTree::getPrefsWidget()
{
    if (!prefsWidget)
    {
        prefsWidget = new AlbumTreePrefsWidget(prefs, this);
        connect(prefsWidget, SIGNAL(destroyed()), this, SLOT(deletePreferences()));
        connect(prefsWidget, SIGNAL(stylesheetChanged(QString)), this, SLOT(setStyleSheet(QString)));
        connect(prefsWidget, SIGNAL(iconChanged(QString)), this, SLOT(fillTree()));
        connect(prefsWidget, SIGNAL(patternChanged(QString)), this, SLOT(fillTree()));
    }

//    prefsWidget->show();
//    prefsWidget->setGeometry(mapToGlobal(QPoint(0,0)).x() + 100, mapToGlobal(QPoint(0,0)).y() + 20, 640, 480);
    return prefsWidget;
}


void AlbumTree::selectedNodeChange(QTreeWidgetItem *cur)
{
    if (prefs->pattern.contains("filepath"))
    {
        QStringList list;
        list.append(cur->text(0));
        while (cur->parent())
        {
            cur = cur->parent();
            list.insert(0, cur->text(0));
        }
        QString s = list.join("/");
        s.insert(0, mlib->libraryPath() + "/");

        mlib->selectTracksBy(Helper::getTags(prefs->pattern).at(0), s);

    } else {
        QStringList tags = Helper::getTags(prefs->pattern);
        QStringList values;
        values.append(cur->text(0));

        while (cur->parent())
        {
            cur = cur->parent();
            values.insert(0, cur->text(0));
        }

        while (tags.size() > values.size())
            tags.removeLast();

        mlib->selectTracksBy(tags, values);
    }
}


void AlbumTree::fillTree()
{
    QMap<QString, int> map = firstNode();
    if (!map.isEmpty())
        mkTree(map);
}



void AlbumTree::mkTree(const QMap<QString, int> &map)
{
    clear();

    QMapIterator<QString, int> i(map);
    QList<QTreeWidgetItem *> list;

    QTreeWidgetItem *item;

    i.next();
    QString key = i.key();

    item = new QTreeWidgetItem(QStringList(key.mid(0, key.indexOf("/"))));
    item->setIcon(0, QIcon(prefs->items_icon));
    list.append(item);
    key.remove(0, item->text(0).size() + 1);

    while (!key.isEmpty())
    {
        QString temp = key.mid(0, key.indexOf("/"));
        QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList(temp));
//        itm->setIcon(0, QIcon(Helper::valueOfTrack("playlistart", pref->music_library_path + "/" + i.key().mid(0, i.key().size() - 1))));
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

        if (key.mid(0, key.indexOf("/")).contains(item->text(0)))
        {
            while (key.mid(0, key.indexOf("/")).contains(item->text(0)))
            {
              //  key.remove(0, item->text(0).size() + 1);
                key.remove(0, item->text(0).size());
                if (key.at(0) == '/')
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
            item = new QTreeWidgetItem(QStringList(key.mid(0, key.indexOf("/"))));
            item->setIcon(0, QIcon(prefs->items_icon));
            list.append(item);
            key.remove(0, item->text(0).size() + 1);
        }


        while (!key.isEmpty())
        {
            QString temp = key.mid(0, key.indexOf("/"));
            QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList(temp));
//            itm->setIcon(0, QIcon(Helper::valueOfTrack("playlistart", pref->music_library_path + "/" + i.key().mid(0, i.key().size() - 1))));
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
    QStringList tags = Helper::getTags(prefs->pattern);

    QSqlQuery query(mlib->db);

    if (query.exec("SELECT id, " + tags.join(", ") + " FROM tracks "
                   "ORDER BY " + tags.join(", ")))
    {
        QMap<QString, int> map;

        if (prefs->pattern.contains("filepath"))
        {
            QString s = mlib->libraryPath() + "/";

            while (query.next())
                map.insert(query.value(1).toString().remove(s) + "/", query.value(0).toInt());
            map.remove(mlib->libraryPath() + "/");
        }
        else
        {
            while (query.next())
            {
                QString s = prefs->pattern;

                for (int i = 0; i < tags.size(); i++)
                {
                    if (!query.value(i+1).toString().isEmpty())
                        s.replace(tags.at(i), query.value(i+1).toString());
                    else
                        s.replace(tags.at(i), tr(QString("Unknown " + tags.at(i)).toUtf8().data()));
                }
                s.remove("%");
                s.append("/");

                map.insert(s, query.value(0).toInt());
            }
        }

        return map;
    }
    //qWarning() << query.lastError();

    QMap<QString, int> map;
    return map;
}

