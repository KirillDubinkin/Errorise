#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QStringList>
//#include "mediadata.h"


class Helper
{
public:
    virtual QString parseLine(const int GUID, QString pattern);
    virtual QString formatTime(int sec) const;
    virtual QString filePath(const int GUID);
    virtual QString fileName(const int GUID);
    virtual int guidOf(QString filename);

    QStringList getTags(QString pattern);

};

#endif // HELPER_H
