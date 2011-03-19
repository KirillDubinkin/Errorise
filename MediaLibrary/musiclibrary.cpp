#include "musiclibrary.h"
#include "paths.h"
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMapIterator>

MusicLibrary::MusicLibrary(const QString &libPath, const QString &filters,
                           QObject *parent) :
    QObject(parent)
{
    sumFiles = 0;

    if (openDb())
    {
        minfo = new PMediaInfo(this);
        connect(minfo, SIGNAL(allFilesScanned(QMultiMap<QString,QMultiMap<QString,QString> >)),
                this, SLOT(insertNewTracks(QMultiMap<QString,QMultiMap<QString,QString> >)));

        fileFilters = filters;
        this->libPath = libPath;

        if (createTagsTable())
        {
            qDebug() << "MusicLib: table doesn't exist! Creating new table..";

            timer.start();

            if (!libPath.isEmpty())
            {
                db.transaction();
                fillDb(libPath);
            }
        }
    }
}


bool MusicLibrary::openDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "MusLibConnect");

    QString path = Paths::iniPath().append(QDir::separator()).append("Music.library");
    path = QDir::toNativeSeparators(path);

    db.setDatabaseName(path);

    return db.open();
}


bool MusicLibrary::createTagsTable()
{
    QString s("CREATE TABLE tracks(");
    s.append("id INTEGER PRIMARY KEY AUTOINCREMENT,");
    s.append("filename TEXT,");
    s.append("artist TEXT,");
    s.append("album TEXT,");
    s.append("albumartist TEXT,");
    s.append("title TEXT,");
    s.append("date TEXT,");
    s.append("tracknumber TEXT,");
    s.append("genre TEXT,");
    s.append("duration TEXT,");
    s.append("format TEXT,");
    s.append("description TEXT,");
    s.append("other TEXT");
    s.append(")");

    QSqlQuery *query = new QSqlQuery(db);
    return query->exec(s);
}


void MusicLibrary::fillDb(QString fromPath)
{
   // qDebug() << "MusLib::fillDb() fromPath:" << fromPath;

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
    minfo->scanFiles(files);
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

        sumFiles++;
    }
    //qDebug() << "#################################################################";


    files.clear();
    if (!dirs.isEmpty())
        fillDb(dirs.dequeue());
    else
    {
        qDebug() << "All diretctories was scanned\n"
                << "There are" << sumFiles << "files\n"
                << "time: " << timer.elapsed() << "msec\n";
        bool ok = db.commit();
        qDebug() << "Commit:" << ok << "\tTime:" << timer.elapsed() << "msec";
    }

}


void MusicLibrary::appendTrack(QString filename, QMultiMap<QString, QString> tags)
{
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO tracks (filename, artist, album, albumartist,"
                   "title, date, tracknumber, genre, duration, format,"
                   "description, other)"
                   "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query->addBindValue(filename);
    query->addBindValue(tags.value("ARTIST"));
    query->addBindValue(tags.value("ALBUM"));
    query->addBindValue(tags.value("ALBUMARTIST"));
    query->addBindValue(tags.value("TITLE"));
    query->addBindValue(tags.value("DATE"));
    query->addBindValue(tags.value("TRACKNUMBER"));
    query->addBindValue(tags.value("GENRE"));
    query->addBindValue(tags.value("DURATION"));
    query->addBindValue(tags.value("FORMAT"));
    query->addBindValue(tags.value("DESCRIPTION"));
    query->addBindValue(tags.value("OTHER"));

    query->exec();
}
