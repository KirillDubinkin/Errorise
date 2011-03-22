#include "simplegui.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>


SimpleGUI::SimpleGUI(QWidget *parent) :
    QDialog(parent)
{
    mainLayout = new QVBoxLayout();

    toolbar = new SimpleToolbar();
    pl      = new SimplePlaylist();
    tree    = new AlbumTree();


    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(tree, 34);
    hl->addWidget(pl, 70);

    mainLayout->setMenuBar(toolbar);
    mainLayout->addLayout(hl, 100);

    this->setLayout(this->mainLayout);
    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);

}

SimpleGUI::~SimpleGUI()
{
   // delete pl;
}


