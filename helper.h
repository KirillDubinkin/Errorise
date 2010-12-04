#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include "mediadata.h"


class Helper
{
public:
    QString parseLine(MediaData *data, QString pattern);

};

#endif // HELPER_H
