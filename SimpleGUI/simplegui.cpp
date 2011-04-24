#include "simplegui.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "version.h"


SimpleGUI::SimpleGUI(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    toolbar = new SimpleToolbar();
    pl      = new SimplePlaylist();
    tree    = new AlbumTree();


    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(tree, 34);
    hl->addWidget(pl, 70);
    hl->setMargin(0);
    hl->setSpacing(6);

//    mainLayout->setMenuBar(toolbar);
    mainLayout->addWidget(toolbar, 0);
    mainLayout->addLayout(hl, 100);

    this->setLayout(this->mainLayout);
    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(myplayerName() + " v." + myplayerVersion());

    connect(player, SIGNAL(trackChanged(QString,int)), this, SLOT(changeTitle()));
}

SimpleGUI::~SimpleGUI()
{
   // delete pl;
}


void SimpleGUI::changeTitle()
{

}
