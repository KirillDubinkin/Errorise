#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

#include <QWidget>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include "global.h"
#include "SimplePlaylist/simpleplaylist.h"
#include "SimpleToolbar/simpletoolbar.h"
#include "MediaLibrary/albumtree.h"
#include "simpleguiprefs.h"

using namespace Global;

class SimpleGUI : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);
    ~SimpleGUI();

    QVBoxLayout *mainLayout;

signals:

public slots:

private slots:
    void changeTitle(QString, int guid);
    void restoreTitle();

private:
    SimpleGUIPrefs *prefs;

    QMenu *mainMenu;
    QWidget *toolBar;
    SimplePlaylist *pl;
    SimpleToolbar *toolbar;

    AlbumTree *tree;
};

#endif // SIMPLEGUI_H
