#include "simplegui.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>

SimpleGUI::SimpleGUI(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);
    this->createToolBar();
}


void SimpleGUI::createToolBar()
{
//    QToolBar *bar = this->addToolBar("asdasdd");


    toolBar = new QWidget(this);
    QSlider * slider = new QSlider(toolBar);
    slider->setOrientation(Qt::Horizontal);


//    bar->addWidget(toolBar);

}
