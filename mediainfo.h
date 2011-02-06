#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QList>
#include "mediadata.h"

class MediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit MediaInfo(QObject *parent = 0);
    ~MediaInfo();

    static int timeToSec(QString time);

    MediaData *track;

  //  QString format;
  //  QString duration;
  //  QString bitrate;

  //  QString title;

    int numParsedFiles;

signals:
    void newTracksReceived(const QList<int> &GUID);

public slots:
    void scanDir(const QString &path);

private:
    QProcess *minfo;

    QStringList out;
    QString line;
    QStringList files;

    QList<int> guid;

    void recursiveDirs(const QString &sDir);
    void parseDir();
    void parse(const QStringList &out, const QStringList &files);

};

#endif // MEDIAINFO_H
