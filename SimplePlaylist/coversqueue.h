#ifndef COVERSQUEUE_H
#define COVERSQUEUE_H

#include <QString>

class CoversQueue
{
public:
    explicit CoversQueue(int artRow, int spanRow, QString filename);

    int     artRow;
    int     spanRow;
    QString filename;

};

#endif // COVERSQUEUE_H
