#include "simplegui.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "helper.h"
#include "version.h"
#include "global.h"

using namespace Global;

SimpleGUI::SimpleGUI(QWidget *parent) :
    QWidget(parent)
{
    prefs           = new SimpleGUIPrefs();
    prefsWidget     = 0;
    sGuiPrefsWidget = 0;
    currentID       = 0;

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    toolbar   = new SimpleToolbar();
    pl        = new SimplePlaylist();
    tree      = new AlbumTree();

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(tree, 25);
    hl->addWidget(pl, 70);
    hl->setMargin(0);
    hl->setSpacing(6);

    mainLayout->addWidget(toolbar, 0);
    mainLayout->addLayout(hl, 100);


    setLayout(this->mainLayout);
    setGeometry(prefs->geometry);
    setWindowFlags(Qt::Window);
    setWindowTitle(myplayerName() + " v." + myplayerVersion());

    connect(player, SIGNAL(trackChanged(QString,int)), this, SLOT(changeTitle(QString,int)));
    connect(player, SIGNAL(finished()), this, SLOT(restoreTitle()));
    connect(player, SIGNAL(tick(qint64)), this, SLOT(setTimeInTitle(qint64)));

    connect(toolbar, SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));
    connect(pl,      SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));
    connect(tree,    SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));

    msgTimer.setSingleShot(true);
    connect(&msgTimer,  SIGNAL(timeout()), this, SLOT(restoreTitle()));
}

SimpleGUI::~SimpleGUI()
{
    prefs->geometry = geometry();
    delete prefs;
}


void SimpleGUI::showMessage(QString msg, int timeout)
{
    msgTimer.stop();
    setWindowTitle(msg);

    if (timeout)
        msgTimer.start(timeout);
}


void SimpleGUI::changeTitle(QString, int guid)
{
    if (!guid)
        return;

    msgTimer.stop();
    currentID = guid;

    if (prefs->title_format.contains("%playbacktime%"))
    {
        isTimeInTitle = true;
        QString temp = prefs->title_format;
        temp.replace("%playbacktime%", "00:00");
        setWindowTitle(Helper::parseLine(currentID, temp));
    }
    else
    {
        isTimeInTitle = false;
        setWindowTitle(Helper::parseLine(currentID, prefs->title_format));
    }
}


void SimpleGUI::setTimeInTitle(qint64 msec)
{
    if (!isTimeInTitle)
        return;

    msgTimer.stop();
    QString temp = prefs->title_format;
    temp.replace("%playbacktime%", QTime(0, 0).addMSecs(msec).toString("mm:ss"));
    setWindowTitle(Helper::parseLine(currentID, temp));
}


void SimpleGUI::restoreTitle()
{
    msgTimer.stop();
    if (player->isStoped())
        setWindowTitle(myplayerName() + " v." + myplayerVersion());

    else if (!isTimeInTitle)
        changeTitle(player->currentTrack(), player->currentGuid());
}


void SimpleGUI::showPreferences()
{
    if (!prefsWidget)
    {
        prefsWidget = new PrefsWidget(this);
        prefsWidget->addPrefsWidget("GUI",           this->getPrefsWidget());
        prefsWidget->addPrefsWidget("Toolbar",       toolbar->getPrefsWidget());
        prefsWidget->addPrefsWidget("Playlist",      pl->getPrefsWidget());
        prefsWidget->addPrefsWidget("Media Library", tree->getPrefsWidget());
        connect(prefsWidget, SIGNAL(geometryChanged(QRect)), this, SLOT(setPrefsGeometry(QRect)));
        connect(prefsWidget, SIGNAL(destroyed()),            this, SLOT(deletePreferences()));
    }

    prefsWidget->setGeometry(prefs->prefs_geometry);
    prefsWidget->show();
}


QWidget * SimpleGUI::getPrefsWidget()
{
    if (!sGuiPrefsWidget)
    {
        if (!sGuiPrefsWidget)
        {
            sGuiPrefsWidget = new SimpleGuiPrefsWidget(prefs, this);
            connect(sGuiPrefsWidget, SIGNAL(windowTitleChanged(QString)), this, SLOT(changeTitle()));
        }
    }

    return sGuiPrefsWidget;
}
