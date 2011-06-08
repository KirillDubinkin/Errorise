#include "simpletoolbar.h"
#include <QSettings>
#include <QTextCodec>
#include <QAction>
#include <QDebug>
#include <QSpacerItem>

#include "global.h"

using namespace Global;


SimpleToolbar::SimpleToolbar(QWidget *parent) :
    QWidget(parent)
{
    prefs = new SimpleToolbarPrefs();
    L = new QHBoxLayout;
    L->setMargin(0);
    setLayout(L);

    setStyleSheet(prefs->style);
    initComponents();
    buildToolbar();
    createMenu();
}

SimpleToolbar::~SimpleToolbar()
{
    delete prefs;
}


void SimpleToolbar::buildToolbar()
{
    deleteComponents();



    for (int i = 0; i < prefs->toolList.size(); i++)
    {
        switch (QString(prefs->toolList.at(i)).toInt())
        {
        case SimpleToolbarPrefs::Seekbar:   L->addWidget(this->seekbar(), 100); break;
        case SimpleToolbarPrefs::Volume:    L->addWidget(this->vol()); break;
        case SimpleToolbarPrefs::Play:      L->addWidget(this->btnPlay()); break;
        case SimpleToolbarPrefs::Pause:     L->addWidget(this->btnPause()); break;
        case SimpleToolbarPrefs::PlayPause: L->addWidget(this->btnPlayPause()); break;
        case SimpleToolbarPrefs::Stop:      L->addWidget(this->btnStop()); break;
        case SimpleToolbarPrefs::Prev:      L->addWidget(this->btnPrev()); break;
        case SimpleToolbarPrefs::Next:      L->addWidget(this->btnNext()); break;
        case SimpleToolbarPrefs::PlaybackOrder: L->addWidget(playbackOrderBox()); break;

        default: L->addSpacing(QString(prefs->toolList.at(i)).toInt());
        }
    }
}


void SimpleToolbar::deleteComponents()
{
    if (seek_bar)       { L->removeWidget(seek_bar);       delete seek_bar; }
    if (volume)         { L->removeWidget(volume);         delete volume; }
    if (btn_next)       { L->removeWidget(btn_next);       delete btn_next; }
    if (btn_pause)      { L->removeWidget(btn_pause);      delete btn_pause; }
    if (btn_play)       { L->removeWidget(btn_play);       delete btn_play; }
    if (btn_play_pause) { L->removeWidget(btn_play_pause); delete btn_play_pause; }
    if (btn_prev)       { L->removeWidget(btn_prev);       delete btn_prev; }
    if (btn_stop)       { L->removeWidget(btn_stop);       delete btn_stop; }
    if (play_order)     { L->removeWidget(play_order);     delete play_order; }

    while (L->count())
        L->removeItem(L->itemAt(0));

    initComponents();
}


void SimpleToolbar::initComponents()
{
    prefsWidget    = 0;

    seek_bar       = 0;
    volume         = 0;

    btn_next       = 0;
    btn_pause      = 0;
    btn_play       = 0;
    btn_play_pause = 0;
    btn_prev       = 0;
    btn_stop       = 0;

    play_order     = 0;
}


QComboBox * SimpleToolbar::playbackOrderBox()
{
    if (play_order == 0) {
        play_order = new QComboBox();
        play_order->addItem(tr("Sequential"));
        play_order->addItem(tr("Repeat track"));
        play_order->addItem(tr("Repeat playlist"));
        play_order->addItem(tr("Shuffle tracks"));
        play_order->addItem(tr("Shuffle albums"));
        play_order->addItem(tr("Random"));
        play_order->setCurrentIndex(pref->playback_order);
        play_order->setContextMenuPolicy(Qt::ActionsContextMenu);

        connect(play_order, SIGNAL(currentIndexChanged(int)), this, SLOT(changePlaybackOrder(int)));
    }

    return play_order;
}


void SimpleToolbar::changePlaybackOrder(int idx)
{
    switch (idx) {
    case 0: pref->playback_order = Preferences::SequentialPlayback; break;
    case 1: pref->playback_order = Preferences::RepeatTrack;        break;
    case 2: pref->playback_order = Preferences::RepeatPlaylist;     break;
    case 3: pref->playback_order = Preferences::ShuffleTracks;      break;
    case 4: pref->playback_order = Preferences::ShuffleAlbums;      break;
    case 5: pref->playback_order = Preferences::RandomPlayback;     break;
    default: qWarning() << "SimpleToolbar::changePlaybackOrder\n\tundefinned index:" << idx;
        break;
    }
}


Phonon::SeekSlider * SimpleToolbar::seekbar()
{
    if (seek_bar == 0) {
        seek_bar = player->seekSlider(this);
        seek_bar->setAttribute(Qt::WA_DeleteOnClose);
    }

    return seek_bar;
}


Phonon::VolumeSlider * SimpleToolbar::vol()
{
    if (volume == 0)
        volume = player->volumeSlider(this);

    return volume;
}


QPushButton * SimpleToolbar::btnNext()
{
    if (btn_next == 0){
        btn_next = new QPushButton(QIcon(prefs->btnNextIcon), prefs->btnNextText);
        btn_next->setMaximumSize(prefs->btnNextWidth, prefs->btnHeight);
        btn_next->setMinimumSize(btn_next->maximumSize());

        connect(btn_next, SIGNAL(clicked()), player, SLOT(next()));
    }

    return btn_next;
}

QPushButton * SimpleToolbar::btnPause()
{
    if (btn_pause == 0){
        btn_pause = new QPushButton(QIcon(prefs->btnPauseIcon), prefs->btnPauseText);
        btn_pause->setMaximumSize(prefs->btnPauseWidth, prefs->btnHeight);
        btn_pause->setMinimumSize(btn_pause->maximumSize());
        connect(btn_pause, SIGNAL(clicked()), player, SLOT(pause()));
    }

    return btn_pause;
}

QPushButton * SimpleToolbar::btnPlay()
{
    if (btn_play == 0){
        btn_play = new QPushButton(QIcon(prefs->btnPlayIcon), prefs->btnPlayText);
        btn_play->setMaximumSize(prefs->btnPlayWidth, prefs->btnHeight);
        btn_play->setMinimumSize(btn_play->maximumSize());
        connect(btn_play, SIGNAL(clicked()), player, SLOT(play()));
    }

    return btn_play;
}

QPushButton * SimpleToolbar::btnPlayPause()
{
    if (btn_play_pause == 0){
        btn_play_pause = new QPushButton(QIcon(prefs->btnPlayPauseIcon), prefs->btnPlayPauseText);
        btn_play_pause->setMaximumSize(prefs->btnPlayPauseWidth, prefs->btnHeight);
        btn_play_pause->setMinimumSize(btn_play_pause->maximumSize());
        connect(btn_play_pause, SIGNAL(clicked()), player, SLOT(playOrPause()));
    }

    return btn_play_pause;
}

QPushButton * SimpleToolbar::btnPrev()
{
    if (btn_prev == 0){
        btn_prev = new QPushButton(QIcon(prefs->btnPrevIcon), prefs->btnPrevText);
        btn_prev->setMaximumSize(prefs->btnPrevWidth, prefs->btnHeight);
        btn_prev->setMinimumSize(btn_prev->maximumSize());

        connect(btn_prev, SIGNAL(clicked()), player, SLOT(prev()));
    }

    return btn_prev;
}

QPushButton * SimpleToolbar::btnStop()
{
    if (btn_stop == 0){
        btn_stop = new QPushButton(QIcon(prefs->btnStopIcon), prefs->btnStopText);
        btn_stop->setMaximumSize(prefs->btnStopWidth, prefs->btnHeight);
        btn_stop->setMinimumSize(btn_stop->maximumSize());
        connect(btn_stop, SIGNAL(clicked()), player, SLOT(stop()));
    }

    return btn_stop;
}


QWidget * SimpleToolbar::getPrefsWidget()
{
    if (!prefsWidget)
    {
        prefsWidget = new SimpeToolbarPrefsWidget(prefs, this);
        connect(prefsWidget, SIGNAL(destroyed()),           this, SLOT(deletePreferences()));
        connect(prefsWidget, SIGNAL(somethingChanged()),    this, SLOT(buildToolbar()));
        connect(prefsWidget, SIGNAL(styleChanged(QString)), this, SLOT(setStyleSheet(QString)));
    }

    return prefsWidget;
}


void SimpleToolbar::createMenu()
{
    setContextMenuPolicy(Qt::ActionsContextMenu);


    QAction *act;

    act = new QAction("Preferences...", this);
    connect(act, SIGNAL(triggered()), this, SIGNAL(needPrefWindow()));
    addAction(act);

}
