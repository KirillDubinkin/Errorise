#include "musiclibrary.h"
#include "pmediainfo.h"
#include "global.h"


#include <QDir>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMapIterator>

using namespace Global;

MusicLibrary::MusicLibrary(const QString &libPath, const QString &filters,
                           QObject *parent) :
    QObject(parent)
{
    ready = false;

    if (!openDb())
    {
        qWarning() << tr("Cannot open or create music library\nPath:") << db.databaseName()
                   << "\n" << tr(db.lastError().text().toUtf8());
        return;
    }


    minfo = new PMediaInfo(this);

    connect(minfo, SIGNAL(allFilesScanned(QMultiMap<QString,QMultiMap<QString,QString> >)),
            this, SLOT(insertNewTracks(QMultiMap<QString,QMultiMap<QString,QString> >)));

    fileFilters = filters;
    this->libPath = libPath;


    if (createTagsTable())       // it means, it's realy first run
       return;


    if (!QDir(libPath).exists()) // path - empty, until user does not change it manually
       return;



    QSqlQuery *query = new QSqlQuery(db);

    if (!query->exec("SELECT COUNT(*) FROM tracks"))
    {
        qWarning() << "MusicLibrary()" << tr(query->lastError().text().toUtf8());
        return;
    }


    query->next();

    if (query->value(0).toInt()) // if db isn't empty
    {
        ready = true;
        emit readyToWork();
        minfo->scanForChanges();

    } else {

        db.transaction();
        fillDb(libPath);
    }
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
    s.append("containerformat TEXT,");
    s.append("comment TEXT,");
    s.append("other TEXT");
    s.append(")");

    QSqlQuery *query = new QSqlQuery(db);
    return query->exec(s);
}


void MusicLibrary::fillDb(QString fromPath)
{
    //qDebug() << "MusLib::fillDb() fromPath:" << fromPath;

    QDir dir(fromPath);
    QFileInfoList list = dir.entryInfoList(fileFilters.split(";"),
                                   QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    foreach(QFileInfo info, list)
    {
        if (info.isDir())
            dirs.enqueue(info.filePath());
        else
            files.append(info.filePath());
    }

//    qDebug() << "MusLib::fillDb() files:" << files;
    if (files.isEmpty() && !dirs.isEmpty())
        return fillDb(dirs.dequeue());

    return minfo->scanFiles(files);
}


void MusicLibrary::insertNewTracks(QMultiMap<QString, QMultiMap<QString, QString> > meta)
{
    QMapIterator<QString, QMultiMap<QString, QString> > i(meta);

    while (i.hasNext())
    {
        i.next();
      //  qDebug() << i.key();
      //  qDebug() << i.value();
        appendTrack(i.key(), i.value());
    }
    //qDebug() << "#################################################################";


    files.clear();
    if (!dirs.isEmpty())
        fillDb(dirs.dequeue());
    else
    {
        db.commit();
        ready = true;
        emit readyToWork();
    }

}


void MusicLibrary::appendTrack(QString filename, QMultiMap<QString, QString> tags)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO tracks (filepath, filename, art, playlistart, modified, artist, album, albumartist,"
                   "title, composer, date, tracknumber, trackcount, genre, duration,"
                   "format, codec, bitrate, channelmode, containerformat,"
                   "comment, other)"
                   "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    QStringList path = filename.split("/");
    path.removeLast();
    query->addBindValue(path.join("/"));
    query->addBindValue(filename.remove(path.join("/")).remove(0, 1));

    query->addBindValue(tags.value("ART"));
    query->addBindValue(tags.value("PLAYLISTART"));
    query->addBindValue(tags.value("MODIFIED"));
    query->addBindValue(tags.value("ARTIST"));
    query->addBindValue(tags.value("ALBUM"));
    query->addBindValue(tags.value("ALBUM-ARTIST"));
    query->addBindValue(tags.value("TITLE"));
    query->addBindValue(tags.value("COMPOSER"));
    query->addBindValue(tags.value("DATE"));
    query->addBindValue(tags.value("TRACK-NUMBER"));
    query->addBindValue(tags.value("TRACK-COUNT"));
    query->addBindValue(tags.value("GENRE"));
    query->addBindValue(tags.value("DURATION"));
    query->addBindValue(tags.value("FORMAT"));
    query->addBindValue(tags.value("AUDIO-CODEC"));
    query->addBindValue(tags.value("BITRATE"));
    query->addBindValue(tags.value("CHANNEL-MODE"));
    query->addBindValue(tags.value("CONTAINER-FORMAT"));
    query->addBindValue(tags.value("COMMENT"));
    query->addBindValue(tags.value("OTHER"));

    query->exec();
}


QString MusicLibrary::libraryPath()
{
    return libPath;
}

bool MusicLibrary::isReady()
{
    return ready;
}


void MusicLibrary::selectTracksBy(QString tag, QString value)
{
    emit tracksSelectedBy(tag, value);
}
