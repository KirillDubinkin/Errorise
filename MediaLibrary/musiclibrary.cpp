#include "musiclibrary.h"
#include "global.h"

#include "mediainfo.h"

#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMapIterator>
#include <QDateTime>
#include <QFile>

using namespace Global;

MusicLibrary::MusicLibrary(const QString &libPath, const QString &filters,
                           QObject *parent) :
    QObject(parent)
{
    ready    = false;
    modified = false;

    updateTimer = new QTimer(this);
    updateTimer->setInterval(pref->lib_update_timeout * 1000);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(checkForUpdates()));

    if (!openDb())
    {
        qWarning() << tr("\n\tCannot open or create music library\nPath:") << db.databaseName()
                   << "\n" << tr(db.lastError().text().toUtf8()) << "\n";
        return;
    }

    createTagsTable();

    minfo = new MediaInfo(this);

    connect(minfo, SIGNAL(newFilesScanned(QMultiMap<QString,QMultiMap<QString,QString> >)),
            this, SLOT(insertNewTracks(QMultiMap<QString,QMultiMap<QString,QString> >)));

    connect(minfo, SIGNAL(oldFilesScanned(QMultiMap<QString,QMultiMap<QString,QString> >)),
            this, SLOT(updateOldTracks(QMultiMap<QString,QMultiMap<QString,QString> >)));

    connect(this, SIGNAL(updateRequired(QStringList)), minfo, SLOT(reScanFiles(QStringList)));
    connect(this, SIGNAL(newFilesAvailable(QStringList)), minfo, SLOT(scanFiles(QStringList)));
    connect(this, SIGNAL(doneWithCurDir()), this, SLOT(checkNextDir()), Qt::QueuedConnection);

    fileFilters = filters;
    this->libPath = libPath;


    if ((libPath.isEmpty()) | (!QDir(libPath).exists())) // path - empty, until user does not change it manually
       return;


    QSqlQuery query(db);

    if (!query.exec("SELECT COUNT(*) FROM tracks"))
    {
        qWarning() << "MusicLibrary(): line 60\n\t" << query.lastError().text() << endl;
        return;
    }


    query.next();

    if (query.value(0).toInt()) // if db isn't empty
    {
        query.clear();
        query.exec("SELECT filepath, modified FROM tracks");
        while (query.next())
        {
            existingFiles.append(query.value(0).toString());
            lastModifiedDates.insert(existingFiles.last(), query.value(1).toLongLong());
        }

        ready = true;
        emit readyToWork();
    }

    QTimer::singleShot(3000, this, SLOT(checkForUpdates()));
}


bool MusicLibrary::openDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "MusLibConnect");
    QString path = QDir::toNativeSeparators(pref->configPath() + "/" + "Music.library");

    db.setDatabaseName(path);

    return db.open();
}


bool MusicLibrary::createTagsTable()
{
    QString s("CREATE TABLE tracks(");
    s.append("id INTEGER PRIMARY KEY AUTOINCREMENT,");
    s.append("filepath TEXT,");
    s.append("filename TEXT,");
    s.append("filedir TEXT,");
    s.append("art TEXT,");
    s.append("playlistart TEXT,");
    s.append("modified TEXT,");
    s.append("artist TEXT,");
    s.append("album TEXT,");
    s.append("albumartist TEXT,");
    s.append("title TEXT,");
    s.append("composer TEXT,");
    s.append("date TEXT,");
    s.append("tracknumber TEXT,");
    s.append("trackcount TEXT,");
    s.append("genre TEXT,");
    s.append("duration TEXT,");
    s.append("format TEXT,");
    s.append("codec TEXT,");
    s.append("bitrate TEXT,");
    s.append("channelmode TEXT,");
    s.append("tagstype TEXT,");
    s.append("comment TEXT");
    s.append(")");

    QSqlQuery query(db);
    return query.exec(s);
}


void MusicLibrary::deleteRemovedFiles()
{
    QSqlQuery query(db);
    query.exec("SELECT filepath FROM tracks");


    db.transaction();

    while (query.next())
    {
        if (!QFile::exists(query.value(0).toString()))
        {
            QSqlQuery deleteQuery(db);
            deleteQuery.exec("DELETE FROM tracks WHERE filepath LIKE '" + query.value(0).toString().replace("'", "''") + "'");

            modified = true;
            existingFiles.removeOne(query.value(0).toString());
            lastModifiedDates.remove(query.value(0).toString());
        }
    }

    db.commit();
}


void MusicLibrary::setLibraryPath(QString path)
{
    libPath = path;

    if (!libPath.isEmpty())
    {
        updateTimer->stop();

        QSqlQuery query(db);
        if (query.exec("DELETE FROM tracks"))
        {
            db.transaction();
            return updateDb(libPath);
        }

        qWarning() << "MusicLibrary::setLibraryPath()\n\t" << query.lastError();
        updateTimer->setInterval(pref->lib_update_timeout * 1000);
        updateTimer->start();
    }
}

void MusicLibrary::checkForUpdates()
{
    if (!libPath.isEmpty())
    {
        updateTimer->stop();
        deleteRemovedFiles();

        db.transaction();
        updateDb(libPath);
    }
}



void MusicLibrary::updateDb(QString fromPath)
{

    QDir dir(fromPath);
    QFileInfoList list = dir.entryInfoList(fileFilters.split(";"),
                                   QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    newFiles.clear();
    QStringList filesToUpdate;

    foreach(QFileInfo info, list)
    {
        if (info.isDir())
            dirs.enqueue(info.filePath());
        else
        {
            if (!existingFiles.contains(info.filePath()))
                newFiles.append(info.filePath());
            else
            if (lastModifiedDates.value(info.filePath()) != info.lastModified().toMSecsSinceEpoch())
                filesToUpdate.append(info.filePath());
        }
    }


    if (newFiles.isEmpty() && filesToUpdate.isEmpty()) //! then, remove all files from current dir
        return emit doneWithCurDir();


    if (!filesToUpdate.isEmpty())
        return emit updateRequired(filesToUpdate);  //! newFilesAvailable will be emited after update

    return emit newFilesAvailable(newFiles);
}



void MusicLibrary::checkNextDir()
{
    if (!dirs.isEmpty())
        return updateDb(dirs.dequeue());

    db.commit();
    ready = true;
    updateTimer->setInterval(pref->lib_update_timeout * 1000);
    updateTimer->start();

    if (modified)
    {
        emit readyToWork();
        modified = false;
    }
}


void MusicLibrary::insertNewTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta)
{
    QMapIterator<QString, QMultiMap<QString, QString> > i(meta);

    while (i.hasNext())
    {
        i.next();
        appendTrack(i.key(), i.value());
    }

    modified = true;
    emit doneWithCurDir();
}


void MusicLibrary::updateOldTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta)
{
    QMapIterator<QString, QMultiMap<QString, QString> > i(meta);

    while (i.hasNext())
    {
        i.next();
        updateTrack(i.key(), i.value());
    }

    if (!newFiles.isEmpty())
        return emit newFilesAvailable(newFiles);

    modified = true;
    emit doneWithCurDir();
}



void MusicLibrary::updateTrack(QString filename, QMultiMap<QString, QString> tags)
{
    QString str("UPDATE tracks SET "
                "art = '"         + QString(tags.value("ART")).replace("'", "''")              + "', "
                "playlistart = '" + QString(tags.value("PLAYLISTART")).replace("'", "''")      + "', "
                "modified = '"    + QString(tags.value("MODIFIED")).replace("'", "''")         + "', "
                "artist = '"      + QString(tags.value("ARTIST")).replace("'", "''")           + "', "
                "album = '"       + QString(tags.value("ALBUM")).replace("'", "''")            + "', "
                "albumartist = '" + QString(tags.value("ALBUM-ARTIST")).replace("'", "''")     + "', "
                "title = '"       + QString(tags.value("TITLE")).replace("'", "''")            + "', "
                "composer = '"    + QString(tags.value("COMPOSER")).replace("'", "''")         + "', "
                "date = '"        + QString(tags.value("DATE")).replace("'", "''")             + "', "
                "tracknumber = '" + QString(tags.value("TRACK-NUMBER")).replace("'", "''")     + "', "
                "trackcount = '"  + QString(tags.value("TRACK-COUNT")).replace("'", "''")      + "', "
                "genre = '"       + QString(tags.value("GENRE")).replace("'", "''")            + "', "
                "duration = '"    + QString(tags.value("DURATION")).replace("'", "''")         + "', "
                "format = '"      + QString(tags.value("FORMAT")).replace("'", "''")           + "', "
                "codec = '"       + QString(tags.value("AUDIO-CODEC")).replace("'", "''")      + "', "
                "bitrate = '"     + QString(tags.value("BITRATE")).replace("'", "''")          + "', "
                "channelmode = '" + QString(tags.value("CHANNEL-MODE")).replace("'", "''")     + "', "
                "tagstype = '"    + QString(tags.value("CONTAINER-FORMAT")).replace("'", "''") + "', "
                "comment = '"     + QString(tags.value("COMMENT")).replace("'", "''")          + "'  "

                "WHERE filepath = '" + filename.replace("'", "''") + "' " );


    QSqlQuery query(db);
    if (query.exec(str)) //! if
    {
        lastModifiedDates.remove(filename);
        lastModifiedDates.insert(filename, QString(tags.value("MODIFIED")).toLongLong());

    } else {

        qWarning() << "MusicLib::updateTrack\n\t" << query.lastError().text() << "\n" << str;
    }
}


void MusicLibrary::appendTrack(QString filename, QMultiMap<QString, QString> tags)
{
    existingFiles.append(filename);
    lastModifiedDates.insert(filename, QString(tags.value("MODIFIED")).toLongLong());

    QSqlQuery query(db);
    query.prepare("INSERT INTO tracks (filepath, filename, filedir, art, playlistart, modified, artist, album, albumartist,"
                   "title, composer, date, tracknumber, trackcount, genre, duration,"
                   "format, codec, bitrate, channelmode, tagstype,"
                   "comment)"
                   "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    QStringList path = filename.split("/");
    path.removeLast();

    query.addBindValue(filename);
    query.addBindValue(filename.remove(path.join("/")).remove(0, 1));
    query.addBindValue(path.join("/"));

    query.addBindValue(tags.value("ART"));
    query.addBindValue(tags.value("PLAYLISTART"));
    query.addBindValue(tags.value("MODIFIED"));
    query.addBindValue(tags.value("ARTIST"));
    query.addBindValue(tags.value("ALBUM"));
    query.addBindValue(tags.value("ALBUM-ARTIST"));
    query.addBindValue(tags.value("TITLE"));
    query.addBindValue(tags.value("COMPOSER"));
    query.addBindValue(tags.value("DATE"));
    query.addBindValue(tags.value("TRACK-NUMBER"));
    query.addBindValue(tags.value("TRACK-COUNT"));
    query.addBindValue(tags.value("GENRE"));
    query.addBindValue(tags.value("DURATION"));
    query.addBindValue(tags.value("FORMAT"));
    query.addBindValue(tags.value("AUDIO-CODEC"));
    query.addBindValue(tags.value("BITRATE"));
    query.addBindValue(tags.value("CHANNEL-MODE"));
    query.addBindValue(tags.value("CONTAINER-FORMAT"));
    query.addBindValue(tags.value("COMMENT"));

    query.exec();
}


QString MusicLibrary::libraryPath()
{
    return libPath;
}

bool MusicLibrary::isReady()
{
    return ready;
}

