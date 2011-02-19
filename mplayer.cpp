#include "mplayer.h"
#include "helper.h"
#include <QDebug>

Mplayer::Mplayer(QueueInterface *q, QObject *parent) :
    PlayerInterface(q, parent)
{
    if (queue)
        qDebug("YEAH!!!");
    else
        qWarning("OH MY GOSH!1!! : queue == 0");

    core = new Core(this);
}



bool Mplayer::play(int guid, int seek)
{
    qDebug("Mplayer::play guid == %d", guid);
    if (guid == -1)
    {
        if (isPaused())
            return pause();

        if (!queue->isEmpty())
            return openNewFile(queue->dequeue(), seek);

        return 0;
    }

    return openNewFile(guid, seek);
}



bool Mplayer::openNewFile(int guid, int seek)
{
    Helper helper;
    core->openFile(helper.filePath(guid), seek);

    state = playerPlaying;
    return 1;
}



bool Mplayer::pause()
{
    int ok = 0;

    switch (state)
    {
    case playerPlaying:
        core->pause();
        state = playerPaused;
        ok = 1;
        break;

    case playerPaused:
        core->pause();
        state = playerPlaying;
        ok = 1;
        break;

    case playerStoped:
        break;

    default:
        qWarning("Something terrible happened.\nPlayer state == %d", state);
        break;
    }

    return ok;
}



bool Mplayer::playNext()
{
    if ( isPlaying() && !queue->isEmpty() )
        return play(queue->dequeue());
    return 0;
}


bool Mplayer::playPrev()
{
    return 0;
}


bool Mplayer::stop()
{
    if (!isPlaying())
        return 0;

    core->stop();
    state = playerStoped;
    emit stoped();

    return 1;
}


bool Mplayer::seek(int sec)
{
    if (sec > -1)
    {
        core->goToSec(sec);
        return 1;
    }

    return 0;
}


bool Mplayer::restart()
{
    return 0;
}
