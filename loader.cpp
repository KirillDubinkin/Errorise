#include "loader.h"
#include "global.h"
#include "SimpleGUI/simplegui.h"

Loader::Loader(QObject *parent): QObject(parent)
{
    gui_p = 0;
    Global::global_init();
}


Loader::~Loader()
{
    delete gui_p;
    Global::global_end();
}


QWidget * Loader::gui()
{
    if (!gui_p)
        gui_p = new SimpleGUI;

    return gui_p;
}


void Loader::start()
{
    gui()->show();
    //! I suppose that someday there will be several GUIs
    //! and function gui() will be decide which show
}


void Loader::processArgs(QStringList args)
{
    // TODO
}
