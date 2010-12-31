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
    mainLayout = new QVBoxLayout;

    toolbar = new SimpleToolbar;
    pl = new SimplePlaylist;

    this->mainLayout->setMenuBar(toolbar);
    this->mainLayout->addWidget(pl, 100);


    this->setLayout(this->mainLayout);


    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);
}

SimpleGUI::~SimpleGUI()
{
    delete pl;
}


