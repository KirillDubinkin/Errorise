#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QStringList>
//#include "mediadata.h"


class Helper
{
public:
    static QString parseLine(const int GUID, QString pattern);
    static QString formatTime(int sec);
    static QString filePath(const int GUID);
    static QString fileName(const int GUID);
    static int guidOf(QString filename);

    static QStringList getTags(QString pattern);
    static QString     getHexColors(int r, int g, int b);

    static QString processContainer(const QString &line, int guid);
    static QMap<int, QString> getQuotes(const QString &line);

};

#endif // HELPER_H
