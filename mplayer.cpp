#include "mplayer.h"

Mplayer::Mplayer(QueueInterface *q, QObject *parent) :
    PlayerInterface(q, parent)
{
    if (queue)
        qDebug("YEAH!!! :%d", *queue);
    else
        qWarning("OH MY GOSH!1!! : queue == 0");

}
