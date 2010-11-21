#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QProcess>
#include "mediadata.h"

class MediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit MediaInfo(QObject *parent = 0);
    ~MediaInfo();

    static int timeToSec(QString time);

    //MediaData *track;
    MediaData *track;

    QString format;
    QString duration;
    QString bitrate;

    QString title;

    int numParsedFiles;

signals:

public slots:
    void start(QString file);
    void parseDir(const QStringList &files);
    void parse(const QStringList &out, const QStringList &files);

private:
    QStringList out;
    QString line;

protected:
    QProcess *minfo;


};

#endif // MEDIAINFO_H
