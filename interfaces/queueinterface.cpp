#include "queueinterface.h"

QueueInterface::QueueInterface(QObject *parent) :
    QObject(parent)
{
    connect(this, SIGNAL(track_removed(int,int)), this, SLOT(removed()));

    connect(this, SIGNAL(track_added(int)), this, SLOT(changeDuration()));
    connect(this, SIGNAL(track_removed(int,int)), this, SLOT(changeDuration()));
}


void QueueInterface::enqueue(int guid)
{
    list.append(guid);
    emit track_added(guid);
}

void QueueInterface::enqueue(QList<int> &guids)
{
    while (!guids.isEmpty())
    {
        list.append(guids.takeFirst());
        emit track_added(list.last());
    }
}

int QueueInterface::dequeue()
{
   if (!list.isEmpty())
    {

       int guid = list.takeFirst();
       emit track_removed(guid, 0);

       return guid;
   }
   return -1;
}

int QueueInterface::head()
{
    if (!list.isEmpty())
        return list.first();
    return -1;
}

int QueueInterface::size()
{
    return list.size();
}


bool QueueInterface::moveUP(int id)
{
    if ((id > 0) & (list.size() <= id))
    {
        list.move(id, id - 1);
        emit track_moved(id, id - 1);
        return true;
    }
    return false;
}

bool QueueInterface::moveDown(int id)
{
    if ((id >= 0) & (list.size() < id))
    {
        list.move(id, id + 1);
        emit track_moved(id, id + 1);
        return true;
    }
    return false;
}

bool QueueInterface::moveTo(int from, int to)
{
    if ( isInList(from) & isInList(to) )
    {
        list.move(from, to);
        emit track_moved(from, to);
        return true;
    }
    return false;
}

bool QueueInterface::remove(int id)
{
    if ( isInList(id) )
    {
        list.removeAt(id);
        emit track_removed(list.at(id), id);

        return true;
    }
    return false;
}


bool QueueInterface::isInList(int id)
{
    return ( (id >= 0) & (id <= list.size()) );
}

int QueueInterface::find(int guid)
{
    return list.indexOf(guid);
}

int QueueInterface::get(int id)
{
    if (isInList(id))
        return list.at(id);
    return -1;
}


QList<int> QueueInterface::get(int from, int to)
{
    QList<int> lst;

    if ( isInList(from) )
    {
        int end = to;

        if ( !isInList(to) )
            end = list.size();

        for(int i = from; i < end; i++)
            lst.append(list.at(i));

        return lst;
    }

    return lst;
}


void QueueInterface::changeDuration()
{
    //! -- some code --

    emit duration_changed(lenght);
}

bool QueueInterface::isEmpty()
{
    return list.isEmpty();
}

int QueueInterface::duration()
{
    return 0;
}


void QueueInterface::removed()
{
    if (list.size() == 1)
        emit one_track_left(list.first());
    else if (list.isEmpty())
        emit queue_is_empty();
}
