#include "phononface.h"
#include "helper.h"

#include "QTime"

using namespace Phonon;

PhononFace::PhononFace(QObject *parent) :
    QObject(parent)
{
    //source = 0;
    output  = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mobject = new Phonon::MediaObject(this);

    path = Phonon::createPath(mobject, output);

    mobject->setTickInterval(1000);

    connect(mobject, SIGNAL(aboutToFinish()), this, SIGNAL(aboutToFinish()));
    connect(mobject, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(mobject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(sourceChange(Phonon::MediaSource)));


    connect(mobject, SIGNAL(tick(qint64)), this, SLOT(printTick(qint64)));

}

void PhononFace::play(int guid)
{
    if (guid == -1)
    {
        if (mobject->state() == Phonon::PausedState)
            return mobject->play();


        if (!mobject->queue().isEmpty())
        {
            qDebug("queue: %s", mobject->queue().first().fileName().toUtf8().data());
            mobject->setCurrentSource(mobject->queue().takeFirst());
            mobject->play();
        }

        return;
    }

    mobject->stop();
    mobject->clearQueue();

    mobject->setCurrentSource(Phonon::MediaSource(Helper().filePath(guid)));

    mobject->play();
}


void PhononFace::enqueue(int guid)
{
    mobject->enqueue(Phonon::MediaSource(Helper().filePath(guid)));
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


void PhononFace::printTick(qint64 tick)
{
    qDebug("Phonon::Tick: %s of %s",
           QTime(0, 0).addMSecs(tick).toString("h:mm:ss").toUtf8().data(),
           QTime(0, 0).addMSecs(mobject->totalTime()).toString("h:mm:ss").toUtf8().data()
           );

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
    return Helper().guidOf(mobject->currentSource().fileName());
}


void PhononFace::sourceChange(Phonon::MediaSource source)
{
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
}
