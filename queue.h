#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include <QList>

#include "mediadata.h"

class Queue
{

public:
    explicit Queue();

    void addQueue(const MediaData data);
    void removeQueue(int idx);
    void clear();
    const MediaData & queue(int idx);

    const MediaData & current();
    int currentIdx();
    bool setCurrent(int idx);

    int size();

private:
    QList<MediaData> md;
    int cur;

};

#endif // QUEUE_H
