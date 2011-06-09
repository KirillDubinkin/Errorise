#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include "minfo.h"

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QQueue>
#include <QMultiMap>
#include <QString>
#include <QStringList>
#include <QTimer>

class MusicLibrary : public QObject
{
    Q_OBJECT
public:
    explicit MusicLibrary(const QString &libPath = "",
                          const QString &filters = "", QObject *parent = 0);

    bool isReady();

    QString       libraryPath();
    void          setLibraryPath(QString path);
    QSqlDatabase  db;

    void selectTracksBy(QStringList tags, QStringList values) { emit tracksSelectedBy(tags, values); }

signals:
    void readyToWork();
    void tracksSelectedBy(QStringList tags, QStringList values);

    void updateRequired(QStringList files);
    void newFilesAvailable(QStringList files);

    void doneWithCurDir();



private slots:
    void insertNewTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta);
    void updateOldTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta);

    void checkNextDir();
    void checkForUpdates();

private:
    Minfo           *minfo;
    QTimer          *updateTimer;

    QString          fileFilters;
    QString          libPath;
    QQueue<QString>  dirs;
    QStringList      newFiles;
    QStringList      existingFiles;

    QMap<QString, qint64> lastModifiedDates;

    bool ready;
    bool modified;
    bool firstRun;

private:
    bool openDb();
    bool createTagsTable();

    void updateDb(QString fromPath);
    void deleteRemovedFiles();

    void appendTrack(QString filename, QMultiMap<QString, QString> tags);
    void updateTrack(QString filename, QMultiMap<QString, QString> tags);

};

#endif // MUSICLIBRARY_H
