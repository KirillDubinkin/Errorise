#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

//#include <QMainWindow>
#include <QDialog>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include "global.h"
#include "SimplePlaylist/simpleplaylist.h"
#include "simpletoolbar.h"
#include "simplealbumlist.h"

#include "MediaLibrary/albumtree.h"

using namespace Global;

class SimpleGUI : public QDialog
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);
    ~SimpleGUI();

    QVBoxLayout *mainLayout;

signals:

public slots:

private:
    QMenu *mainMenu;
    QWidget *toolBar;
    SimplePlaylist *pl;
    SimpleToolbar *toolbar;
    SimpleAlbumlist *alist;

    AlbumTree *tree;
};

#endif // SIMPLEGUI_H
