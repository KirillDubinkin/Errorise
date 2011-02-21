#ifndef HELPER_H
#define HELPER_H

#include <QString>
//#include "mediadata.h"


class Helper
{
public:
    virtual QString parseLine(const int GUID, QString pattern) const;
    virtual QString formatTime(int sec) const;
    virtual QString filePath(const int GUID);
    virtual QString fileName(const int GUID);
    virtual int guidOf(const QString &filename);

};

#endif // HELPER_H
