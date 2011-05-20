#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

#include <QWidget>

#include "SimplePlaylist/simpleplaylist.h"
#include "SimpleToolbar/simpletoolbar.h"
#include "MediaLibrary/albumtree.h"
#include "simpleguiprefs.h"
#include "prefswidget.h"

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
    void setTimeInTitle(qint64 msec);

private:
    SimpleGUIPrefs *prefs;
    PrefsWidget    *prefsWidget;

    SimplePlaylist *pl;
    SimpleToolbar  *toolbar;
    AlbumTree      *tree;

    bool isTimeInTitle;
    int  currentID;
};

#endif // SIMPLEGUI_H
