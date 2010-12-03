#include "queue.h"

#include <QDebug>
Queue::Queue()
{
    cur = 0;
}

void Queue::addQueue(const MediaData data)
{
    md.append(data);
    qDebug() << md.size() << ":" << md.at(md.size()-1).clip_name;
}

void Queue::removeQueue(int idx)
{
    if (md.size() >= idx)
        md.removeAt(idx);
}

const MediaData & Queue::queue(int idx)
{
    if (md.size() > idx)
        return md.at(idx);
    return md.first();
}

const MediaData & Queue::current()
{
    if (md.size() > cur)
        return md.at(cur);
    return md.first();
}

int Queue::currentIdx()
{
    return cur;
}

bool Queue::setCurrent(int idx)
{
    if (md.size() >= idx){
        cur = idx;
        return true;
    }
    return false;
}

int Queue::size()
{
    return md.size();
}

void Queue::clear()
{
    md.clear();
    cur = 0;
}
