#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include "mediainfo.h"

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QQueue>
#include <QMultiMap>
#include <QString>
#include <QStringList>
#include <QTime>

class MusicLibrary : public QObject
{
    Q_OBJECT
public:
    explicit MusicLibrary(const QString &libPath = "",
                          const QString &filters = "", QObject *parent = 0);

    bool isReady();

    QString       libraryPath();
    QSqlDatabase  db;

    void selectTracksBy(QString tag, QString value);

signals:
    void readyToWork();
    void tracksSelectedBy(QString tag, QString value);

public slots:



private slots:
    void insertNewTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta);

private:
    MediaInfo       *minfo;
    QString          fileFilters;
    QString          libPath;
    QQueue<QString>  dirs;
    QStringList      files;

    bool ready;

    QTime timer;

private:
    bool openDb();
    bool createTagsTable();
    void fillDb(QString fromPath);
    void appendTrack(QString filename, QMultiMap<QString, QString> tags);

};

#endif // MUSICLIBRARY_H
