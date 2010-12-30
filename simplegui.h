#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

//#include <QMainWindow>
#include <QDialog>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include "global.h"
#include "simpleplaylist.h"
#include "myslider.h"

using namespace Global;

class SimpleGUI : public QDialog
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);
    ~SimpleGUI();

    QVBoxLayout *mainLayout;
    MySlider *progress, *vol;

signals:

public slots:

private:
    QMenu *mainMenu;
    QWidget *toolBar;
    SimplePlaylist *pl;

    void createToolBar();

};

#endif // SIMPLEGUI_H
