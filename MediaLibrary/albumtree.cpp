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

        mlib->selectTracksBy(QStringList("filepath"), QStringList(s));

        qDebug() << "AlbumTree::selectedNodeChange\n\titem:" << cur->text(0)
                 << "\n\ttags:" << QStringList("filepath")
                 << "\n\tvalues:" << QStringList(s) << endl;

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
        {
            qDebug() << "AlbumTree::selectedNodeChange\n\ttags > values:" << tags.last() << endl;
            tags.removeLast();
        }

        mlib->selectTracksBy(tags, values);

        qDebug() << "AlbumTree::selectedNodeChange\n\titem:" << cur->text(0)
                 << "\n\ttags:" << tags
                 << "\n\tvalues:" << values << endl;
    }
}


void AlbumTree::fillTree()
{
    QMap<QString, QString> map = firstNode();

    QMapIterator<QString, QString> i(map);

    for (int j = 0; j < 1000; j++)
    {
        i.next();
        qDebug("%3d:   %s", j, i.key().toUtf8().data());
    }

    if (!map.isEmpty())
        mkTree(map);


}



void AlbumTree::mkTree(const QMap<QString, QString> &map)
{
    clear();

    QMapIterator<QString, QString> i(map);
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

        if (prefs->context_icons)
            itm->setIcon(0, QIcon(i.value()));
        else
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

            if (prefs->context_icons)
                itm->setIcon(0, QIcon(i.value()));
            else
                itm->setIcon(0, QIcon(prefs->items_icon));

            item->addChild(itm);
            item = itm;
            key.remove(0, temp.size() + 1);
        }
    }

    this->invisibleRootItem()->addChildren(list);
}



QMap<QString, QString> AlbumTree::firstNode()
{
    QStringList tags = Helper::getTags(prefs->pattern);

    QSqlQuery query(mlib->db);

    if (query.exec("SELECT playlistart, " + tags.join(", ") + " FROM tracks "
                   "ORDER BY " + tags.join(", ")))
    {
        QMap<QString, QString> map;

        if (prefs->pattern.contains("filepath"))
        {
            QString s = mlib->libraryPath() + "/";

            while (query.next())
                map.insert(query.value(1).toString().remove(s) + "/", query.value(0).toString());
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

                map.insert(s, query.value(0).toString());
            }
        }

        return map;
    }
    //qWarning() << query.lastError();

    QMap<QString, QString> map;
    return map;
}

