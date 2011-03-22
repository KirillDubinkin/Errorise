#include "simpletoolbar.h"
#include "paths.h"
#include <QSettings>
#include <QTextCodec>
#include <QAction>
#include <QDebug>

#include "global.h"

using namespace Global;


SimpleToolbar::SimpleToolbar(QWidget *parent) :
    QWidget(parent)
{
    prefs = new SimpleToolbarPrefs();
    L = new QHBoxLayout;
    L->setMargin(0);

    this->setStyleSheet(prefs->style);
    this->initComponents();

    for (int i = 0; i < prefs->toolList.size(); i++)
    {
        switch (QString(prefs->toolList.at(i)).toInt())
        {
        case Seekbar: L->addWidget(this->seekbar(), 100); break;
        case Volume: L->addWidget(this->vol()); break;
        case Play: L->addWidget(this->btnPlay()); break;
        case Pause: L->addWidget(this->btnPause()); break;
        case PlayPause: L->addWidget(this->btnPlayPause()); break;
        case Stop: L->addWidget(this->btnStop()); break;
        case Prev: L->addWidget(this->btnPrev()); break;
        case Next: L->addWidget(this->btnNext()); break;

        default: L->addSpacing(QString(prefs->toolList.at(i)).toInt());
        }
    }

    this->setLayout(L);
}

SimpleToolbar::~SimpleToolbar()
{
    delete prefs;
}

void SimpleToolbar::initComponents()
{
    seek_bar = 0;
    volume = 0;
    small_menu = 0;
    full_menu = 0;
    playback_order = 0;

    btn_next = 0;
    btn_pause = 0;
    btn_play = 0;
    btn_play_pause = 0;
    btn_prev = 0;
    btn_stop = 0;
}

Phonon::SeekSlider * SimpleToolbar::seekbar()
{
    if (seek_bar == 0) {
        //seek_bar = new Phonon::SeekSlider(player->mediaObject(), this);
        seek_bar = player->seekSlider(this);
        seek_bar->setAttribute(Qt::WA_DeleteOnClose);
        seek_bar->setContextMenuPolicy(Qt::ActionsContextMenu);

        QAction *act;
        seek_bar->addAction(act = new QAction(tr("Remove from Toolbar"), seek_bar));
        connect(act, SIGNAL(triggered()), this, SLOT(hideSeekbar()));
    }

    return seek_bar;
}

void SimpleToolbar::hideSeekbar()
{
    if (prefs->toolList.removeOne(QString::number(Seekbar))) {
        this->seekbar()->close();
    }
}

Phonon::VolumeSlider * SimpleToolbar::vol()
{
    if (volume == 0){
        //volume = new Phonon::VolumeSlider(player->audioOutput(), this);
        volume = player->volumeSlider(this);
/*
        volume->setMinimum(0);
        volume->setMaximum(100);
*/
        volume->setContextMenuPolicy(Qt::ActionsContextMenu);
        volume->setAttribute(Qt::WA_DeleteOnClose);

        QAction *act;
        volume->addAction(act = new QAction(tr("Remove from toolbar"), volume));
        connect(act, SIGNAL(triggered()), this, SLOT(hideVol()));

    }

    return volume;
}

void SimpleToolbar::hideVol()
{
    if (prefs->toolList.removeOne(QString::number(Volume))) {
       // L->removeWidget(this->vol());
        this->vol()->close();
    }
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
