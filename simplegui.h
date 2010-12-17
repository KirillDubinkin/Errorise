#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include "global.h"

using namespace Global;

class SimpleGUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);

signals:

public slots:

private:
    QMenu *mainMenu;
    QWidget *toolBar;

    void createToolBar();

};

#endif // SIMPLEGUI_H
