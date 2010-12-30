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

    this->createToolBar();


    pl = new SimplePlaylist;
    this->mainLayout->addWidget(pl, 100);


    this->setLayout(this->mainLayout);



   // this->mainLayout->addWidget(pl);
  //  this->setLayout(mainLayout);

  //  this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);


}

SimpleGUI::~SimpleGUI()
{
    delete pl;
}


void SimpleGUI::createToolBar()
{
//    QToolBar *bar = this->addToolBar("asdasdd");


    toolBar = new QWidget;
    QHBoxLayout *L = new QHBoxLayout;

    progress = new MySlider;

    vol = new MySlider();
    vol->setMinimum(0);
    vol->setMaximum(100);
    vol->setValue(pref->volume);

    L->addWidget(progress, 100);
    L->addWidget(vol);

    toolBar->setLayout(L);
    //toolBar->setLayout(L);

    //this->mainLayout->setMenuBar(toolBar);

    this->mainLayout->setMenuBar(toolBar);

//    bar->addWidget(toolBar);

}
