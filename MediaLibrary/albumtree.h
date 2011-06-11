#ifndef ALBUMTREE_H
#define ALBUMTREE_H

#include <QTreeWidget>
#include <QStringList>
#include <QMultiMap>
#include <QModelIndex>

#include "MediaLibrary/albumtreeprefs.h"
#include "MediaLibrary/albumtreeprefswidget.h"
#include "global.h"

using namespace Global;

class AlbumTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit AlbumTree(QWidget *parent = 0);
    ~AlbumTree();

    QWidget *getPrefsWidget();

signals:
    void about();

private slots:
    void fillTree();
    void createPrefsWidget();
    void deletePreferences() { prefsWidget = 0; }

    void selectedNodeChange(QTreeWidgetItem *cur);

private:
    void createMenu();
    QMap<QString, QString> firstNode();
    void mkTree(const QMap<QString, QString> &map);

private:
    AlbumTreePrefs          *prefs;
    AlbumTreePrefsWidget    *prefsWidget;


};

#endif // ALBUMTREE_H
