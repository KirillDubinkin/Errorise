#ifndef SIMPLEGUI_H
#define SIMPLEGUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QRect>
#include <QTimer>

#include "SimplePlaylist/simpleplaylist.h"
#include "SimpleToolbar/simpletoolbar.h"
#include "MediaLibrary/albumtree.h"
#include "simpleguiprefs.h"
#include "prefswidget.h"
#include "simpleguiprefswidget.h"

class SimpleGUI : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleGUI(QWidget *parent = 0);
    ~SimpleGUI();

    QVBoxLayout *mainLayout;
    QWidget *getPrefsWidget();

signals:

public slots:
    void showPreferences(QWidget *defaultWidget = 0);
    void showMessage(QString msg, int timeout = 2000);

private slots:
    void changeTitle(QString, int guid);
    void changeTitle()  { changeTitle(QString::null, currentID); }

    void restoreTitle();
    void deletePreferences()              { prefsWidget = 0; sGuiPrefsWidget = 0; }
    void setPrefsGeometry(QRect geomerty) { prefs->prefs_geometry = geomerty; }
    void setTimeInTitle(qint64 msec);

private:
    SimpleGUIPrefs       *prefs;
    PrefsWidget          *prefsWidget;
    SimpleGuiPrefsWidget *sGuiPrefsWidget;

    SimplePlaylist *pl;
    SimpleToolbar  *toolbar;
    AlbumTree      *tree;

    QTimer msgTimer;

    bool   isTimeInTitle;
    int    currentID;
};

#endif // SIMPLEGUI_H
