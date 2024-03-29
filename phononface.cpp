#include "phononface.h"
#include "helper.h"
#include "global.h"
#include "SimpleGUI/simplegui.h"

#include "QTime"
#include <QDebug>

using namespace Phonon;

PhononFace::PhononFace(QObject *parent) :
    QObject(parent)
{
    isPreviousPlaying = false;

    output  = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mobject = new Phonon::MediaObject(this);

    path = Phonon::createPath(mobject, output);

    mobject->setTickInterval(1000);

    connect(mobject, SIGNAL(tick(qint64)),    this, SIGNAL(tick(qint64)));
    connect(mobject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
    connect(mobject, SIGNAL(finished()),      this, SIGNAL(finished()));
    connect(mobject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this,    SLOT(sourceChange(Phonon::MediaSource)));

    connect(mobject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChange(Phonon::State,Phonon::State)));

    connect(output, SIGNAL(volumeChanged(qreal)), this, SIGNAL(volumeChanged(qreal)));

}


bool PhononFace::isStoped()
{
    if ( (mobject->state() == Phonon::PlayingState) | (mobject->state() == Phonon::PausedState) )
        return false;

    return true;
}


void PhononFace::stateChange(Phonon::State cur, Phonon::State)
{
    switch (cur) {
/*    case Phonon::LoadingState:
        qDebug() << "Phonon::loading\t" << mobject->currentSource().fileName();
        break;
    case Phonon::StoppedState:
        qDebug() << "Phonon::stoped\t" << mobject->currentSource().fileName();
        break;
    case Phonon::PlayingState:
        qDebug() << "Phonon::playing\t" << mobject->currentSource().fileName();
        break;
    case Phonon::BufferingState:
        qDebug() << "Phonon::burrefing\t" << mobject->currentSource().fileName();
        break;
    case Phonon::PausedState:
        qDebug() << "Phonon::paused\t" << mobject->currentTime();
        break; */
    case Phonon::ErrorState:
        QString error = mobject->errorString();
        qWarning() << "Phonon::error\n\ttype:" << mobject->errorType()
                   << "\n\ttext:" << error
                   << "\n\tfile:" << mobject->currentSource().fileName() << endl;
        Global::gui->showMessage(error.mid(0, error.lastIndexOf("(")), 8000);
    }
}


void PhononFace::play(int guid)
{
    if (guid == -1)
    {
        if (mobject->state() == Phonon::PausedState)
            return mobject->play();


        if (!mobject->queue().isEmpty())
        {
            mobject->setCurrentSource(mobject->queue().takeFirst());
            mobject->play();
        }

        return;
    }

    mobject->stop();
    mobject->clearQueue();

    mobject->setCurrentSource(Phonon::MediaSource(Helper().fileName(guid)));

    mobject->play();
}


void PhononFace::enqueue(int guid)
{
    mobject->enqueue(Phonon::MediaSource(Helper().fileName(guid)));
}


void PhononFace::enqueue(QList<int> guids)
{
    while (!guids.isEmpty())
        this->enqueue(guids.takeFirst());
}


void PhononFace::pause()
{
    if (mobject->state() == Phonon::PausedState)
        return mobject->play();
    mobject->pause();
}


void PhononFace::playOrPause()
{
    if (mobject->state() == Phonon::PlayingState)
        return mobject->pause();

    this->play();
}


void PhononFace::stop()
{
    mobject->stop();
    emit finished();
}


bool PhononFace::isSekable()
{
    return mobject->isSeekable();
}


void PhononFace::seek(int sec)
{
    mobject->seek(sec*1000);
}


QString PhononFace::currentTrack()
{
    return mobject->currentSource().fileName();
}

int PhononFace::currentGuid()
{
    if ((mobject->state() == Phonon::PlayingState) || (mobject->state() == Phonon::PausedState))
        return Helper().guidOf(mobject->currentSource().fileName());
    return -1;
}


void PhononFace::sourceChange(Phonon::MediaSource source)
{
    if (currentSource.type() != Phonon::MediaSource::Empty)
    {
            // In here, this->currentSource still contain previos track
        if (!isPreviousPlaying)
        {
            prevTracks.append(currentSource);
//            qDebug() << "PhononFace: Added previous track:" << prevTracks.last().fileName();
        }
        isPreviousPlaying = false;
    }

    currentSource = source;

    emit trackChanged(source.fileName(), Helper().guidOf(source.fileName()));
}


void PhononFace::next()
{
    if (!mobject->queue().isEmpty())
    {
        mobject->stop();
        mobject->setCurrentSource(mobject->queue().takeFirst());
        return mobject->play();
    }

    emit needNext();
      //! We tells everyone, that we want
      //! And maybe someone, like playlist, help us
}


void PhononFace::prev()
{
    if (!prevTracks.isEmpty())
    {
        mobject->stop();

        QList<Phonon::MediaSource> temp;
        temp.append(mobject->currentSource());
            //! We add current, and all queue tracks to new queue

        for (int i = 0; i < mobject->queue().size(); i++)
            temp.append(mobject->queue().at(i));

        mobject->clearQueue();

        isPreviousPlaying = true;
            //! It's changes to false on CurrentSourceChanged() signal
            //! It's easy way, to avoid adding current-previos track to prevTracks

        mobject->setCurrentSource(Phonon::MediaSource(prevTracks.takeLast().fileName()));
        mobject->play();

        while (!temp.isEmpty())
            mobject->enqueue(temp.takeFirst());
    }
}


Phonon::VolumeSlider * PhononFace::volumeSlider(QWidget *parent)
{
    return new Phonon::VolumeSlider(output, parent);
}


Phonon::SeekSlider * PhononFace::seekSlider(QWidget *parent)
{
    return new Phonon::SeekSlider(mobject, parent);
}

