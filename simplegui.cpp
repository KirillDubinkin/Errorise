#include "simplegui.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "myslider.h"


SimpleGUI::SimpleGUI(QWidget *parent) :
    QDialog(parent)
{
    mainLayout = new QVBoxLayout(this);

    toolbar = new SimpleToolbar(this);
    pl = new SimplePlaylist(this);
    alist = new SimpleAlbumlist(this);

    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->addWidget(alist, 30);
    hl->addWidget(pl, 70);

    this->mainLayout->setMenuBar(toolbar);
    this->mainLayout->addLayout(hl, 100);
    //this->mainLayout->addWidget(alist, 20);
    //this->mainLayout->addWidget(pl, 80);



    this->setLayout(this->mainLayout);


    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);
}

SimpleGUI::~SimpleGUI()
{
   // delete pl;
}


