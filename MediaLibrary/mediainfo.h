#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QProcess>
#include <QMap>
#include <QMultiMap>
#include <QStringList>
#include "minfo.h"

class MediaInfo : public Minfo
{
    Q_OBJECT
public:
    explicit MediaInfo(QObject *parent = 0);
    ~MediaInfo();

    static int timeToSec(QString time);

    QMap<QString, QMultiMap<QString, QString> > meta;

    QString format;
    QString duration;
    QString bitrate;

    QString title;

    int numParsedFiles;

signals:
    void fileScanned();

public slots:
    void scanFiles(QStringList files);
    void scanFile(QString filename);
    QMultiMap<QString, QString> metadata();


private slots:
    void pringTags();
    void scanNextFile();

private:
    QString     artFilePath;
    QString     playlistArtFilePath;
    QString     currentFile;
    QStringList filenames;

    bool        isUpdateState;

protected:
    QProcess *minfo;


};

#endif // MEDIAINFO_H
