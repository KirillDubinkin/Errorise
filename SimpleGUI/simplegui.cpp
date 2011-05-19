#include "simplegui.h"
#include "helper.h"
#include <QAction>
#include <QToolBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "version.h"
#include <QDebug>


SimpleGUI::SimpleGUI(QWidget *parent) :
    QWidget(parent)
{
    prefs       = new SimpleGUIPrefs();
    prefsWidget = 0;

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    toolbar = new SimpleToolbar();
    pl      = new SimplePlaylist();
    tree    = new AlbumTree();


    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(tree, 25);
    hl->addWidget(pl, 70);
    hl->setMargin(0);
    hl->setSpacing(6);

//    mainLayout->setMenuBar(toolbar);
    mainLayout->addWidget(toolbar, 0);
    mainLayout->addLayout(hl, 100);

    this->setLayout(this->mainLayout);
    this->setGeometry(prefs->geometry);
    this->setWindowFlags(Qt::Window);
    this->setWindowTitle(myplayerName() + " v." + myplayerVersion());

    connect(player, SIGNAL(trackChanged(QString,int)), this, SLOT(changeTitle(QString,int)));
    connect(player, SIGNAL(finished()), this, SLOT(restoreTitle()));

    connect(toolbar, SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));
    connect(pl,      SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));
    connect(tree,    SIGNAL(needPrefWindow()), this, SLOT(showPreferences()));
}

SimpleGUI::~SimpleGUI()
{
    prefs->geometry = this->geometry();
    delete prefs;
}


void SimpleGUI::changeTitle(QString, int guid)
{
    this->setWindowTitle(Helper::parseLine(guid, prefs->title_format));
    qDebug() << "GUID:" << guid;
}


void SimpleGUI::restoreTitle()
{
    this->setWindowTitle(myplayerName() + " v." + myplayerVersion());
}


void SimpleGUI::showPreferences()
{
    if (!prefsWidget)
    {
        prefsWidget = new PrefsWidget(this);
        prefsWidget->addPrefsWidget("Toolbar", toolbar->getPrefsWidget());
        prefsWidget->addPrefsWidget("Playlist", pl->getPrefsWidget());
        prefsWidget->addPrefsWidget("Media Library", tree->getPrefsWidget());
    }

    prefsWidget->setGeometry(mapToGlobal(QPoint(0,0)).x() + 100, mapToGlobal(QPoint(0,0)).y() + 20, 800, 576);
    prefsWidget->show();
}
