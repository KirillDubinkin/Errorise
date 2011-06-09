#include "simplegui.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QList>
#include <QTime>

#include "helper.h"
#include "global.h"

using namespace Global;

SimpleGUI::SimpleGUI(QWidget *parent) :
    QWidget(parent)
{
    qDebug("Load GUI");
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
    setWindowTitle(qApp->applicationName() + " v." + qApp->applicationVersion());

    connect(player, SIGNAL(trackChanged(QString,int)), this, SLOT(changeTitle(QString,int)));
    connect(player, SIGNAL(finished()), this, SLOT(restoreTitle()));
    connect(player, SIGNAL(tick(qint64)), this, SLOT(setTimeInTitle(qint64)));

    msgTimer.setSingleShot(true);
    connect(&msgTimer,  SIGNAL(timeout()), this, SLOT(restoreTitle()));
}

SimpleGUI::~SimpleGUI()
{
    qDebug("Delete GUI");
    prefs->geometry = geometry();
    delete prefs;
}


void SimpleGUI::showMessage(QString msg, int timeout)
{
    msgTimer.stop();
    setWindowTitle(qApp->applicationName() + ": " + msg);

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
        setWindowTitle(qApp->applicationName() + " v." + qApp->applicationVersion());

    else if (!isTimeInTitle)
        changeTitle(player->currentTrack(), player->currentGuid());
}


void SimpleGUI::showPreferences(QWidget *defaultWidget)
{
    if (prefsWidget)
        return;

    prefsWidget = (PrefsWidget*) 1; // ugly huck

    QList<QWidget *> prefsList;
    prefsList.append(this->getPrefsWidget());
    prefsList.append(toolbar->getPrefsWidget());
    prefsList.append(pl->getPrefsWidget());
    prefsList.append(tree->getPrefsWidget());

    prefsWidget = new PrefsWidget(prefsList, defaultWidget, this);
    prefsWidget->setGeometry(prefs->prefs_geometry);

    connect(prefsWidget, SIGNAL(geometryChanged(QRect)), this, SLOT(setPrefsGeometry(QRect)));
    connect(prefsWidget, SIGNAL(destroyed()),            this, SLOT(deletePreferences()));

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
