#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

#include <QWidget>
#include <QMenu>
#include <QWidget>
#include <QVBoxLayout>
#include "global.h"
#include "SimplePlaylist/simpleplaylist.h"
#include "SimpleToolbar/simpletoolbar.h"
#include "MediaLibrary/albumtree.h"
#include "simpleguiprefs.h"
#include "prefswidget.h"

using namespace Global;

class SimpleGUI : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);
    ~SimpleGUI();

    QVBoxLayout *mainLayout;

signals:

public slots:
    void showPreferences();

private slots:
    void changeTitle(QString, int guid);
    void restoreTitle();
    void deletePreferences() { prefsWidget = 0; }

private:
    SimpleGUIPrefs *prefs;
    PrefsWidget    *prefsWidget;

    QMenu *mainMenu;
    SimplePlaylist *pl;
    SimpleToolbar *toolbar;

    AlbumTree *tree;
};

#endif // SIMPLEGUI_H
