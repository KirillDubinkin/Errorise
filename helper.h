#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QStringList>
//#include "mediadata.h"


class Helper
{
public:
    static QString parseLine(const int id, QString pattern);
    static QString formatTime(int sec);
    static QString filePath(const int id);
    static QString fileName(const int id);
    static int guidOf(QString filename);


    static QString     getHexColors(int r, int g, int b);

    static QStringList getQuotes(const QString &line);
    static QStringList getTags(QString pattern);
    static QString     getTag(const QString &pattern);

    static int nextQuote(const QString &line, int from);

    static QString     valueOfTrack(const QString &tag, const int id);
    static QStringList valueOfTrack(const QStringList &tags, const int id);


    static QString processQuotes(QString line);
    static QString processTags(QString line, const int id);
    static QString processContainer(QString line, const int id);



};

#endif // HELPER_H
