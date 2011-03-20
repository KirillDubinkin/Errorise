#include "simplegui.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>


SimpleGUI::SimpleGUI(QWidget *parent) :
    QDialog(parent)
{
    mainLayout = new QVBoxLayout(this);

    toolbar = new SimpleToolbar(this);
    pl = new SimplePlaylist(this);
//    alist = new SimpleAlbumlist(this);
    tree = new AlbumTree(this);


    QHBoxLayout *hl = new QHBoxLayout(this);
    //hl->addWidget(alist, 30);
    hl->addWidget(tree, 34);
    hl->addWidget(pl, 70);

    this->mainLayout->setMenuBar(toolbar);
    this->mainLayout->addLayout(hl, 100);

    this->setLayout(this->mainLayout);
    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);


    //connect(alist, SIGNAL(directoryChanged(QString)), mediainfo, SLOT(scanDir(QString)));
}

SimpleGUI::~SimpleGUI()
{
   // delete pl;
}


