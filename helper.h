#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QStringList>

class Helper
{
public:
    static QString filePath(const int id);
    static QString fileName(const int id);
    static int guidOf(QString filename);

    static QStringList getTags(QString pattern);
    static QString     getTag(const QString &pattern);

    static QStringList valueOfTrack(const QStringList &tags, const int id);
    static QString     valueOfTrack(const QString &tag, const int id);
    static QString     valueOfTrack(const QString &tag, QString filePath);

    static QString parseLine(const int id, QString line);


protected:
    static int  nextQuote(const QString &line, int from);
    static bool isContainer(const QString &line);
    static int  funcEnd(const QString &line, const int from = 0);
    static int  funcSize(const QString &line, const int from = 0);
    static QStringList getQuotes(const QString &line);

    static QString processQuotes(QString line);
    static QString processTags(QString line, const int id);
    static QString processContainer(QString line, const int id);
    static QString processContainers(QString line, const int id);
    static QString processFunctions(QString line, const int id);

    static QString doFunc(const QString &func, QString line, const int id);
    static QString funcIF(QString line, const int id);
    static QString funcIF2(QString line, const int id);
};

#endif // HELPER_H
