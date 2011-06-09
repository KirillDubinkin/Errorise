#include "mediainfo.h"

#include <QDebug>
#include <QRegExp>
#include <QTime>
#include <QFileInfo>
#include <QTextCodec>

MediaInfo::MediaInfo(QObject *parent): Minfo(parent)
{
    qDebug("Load MediaInfo");
    minfo = new QProcess(this);

    connect(minfo, SIGNAL(finished(int)), this, SLOT(pringTags()));
    connect(this,  SIGNAL(fileScanned()), this, SLOT(scanNextFile()));
    connect(minfo, SIGNAL(error(QProcess::ProcessError)), this, SLOT(gotError(QProcess::ProcessError)));
}


void MediaInfo::gotError(QProcess::ProcessError error)
{
    qWarning() << "MediaInfo got error!\n"
               << "\terror:" << error
               << "\tmessage:" << minfo->errorString() << endl;

    if ( (error == QProcess::FailedToStart) | (error == QProcess::Crashed) )
        emit failedToStart();
}


void MediaInfo::scanFiles(QStringList files)
{
    isUpdateState = false;

    qDebug() << "MediaInfo::scanFiles\tpath:" << files.first().mid(0, files.first().lastIndexOf("/"));

    playlistArtFilePath.clear();
    artFilePath.clear();

    playlistArtFilePath = findPlArt(QFileInfo(files.first()).absoluteDir());

    if (playlistArtFilePath.isEmpty())
        artFilePath     = findArt(QFileInfo(files.first()).absolutePath());


    filenames = files;
    meta.clear();
    scanNextFile();
}


void MediaInfo::reScanFiles(QStringList files)
{
    isUpdateState = true;

    qDebug() << "MediaInfo::reScanFiles\tpath:" << files.first().mid(0, files.first().lastIndexOf("/"));

    playlistArtFilePath.clear();
    artFilePath.clear();

    playlistArtFilePath = findPlArt(QFileInfo(files.first()).absoluteDir());
    if (playlistArtFilePath.isEmpty())
        artFilePath     = findArt(QFileInfo(files.first()).absolutePath());

    filenames = files;
    meta.clear();
    scanNextFile();
}


void MediaInfo::scanNextFile()
{
    if (filenames.size())
        scanFile(filenames.takeFirst());
    else
    {
        if (isUpdateState)
            emit oldFilesScanned(meta);
        else
            emit newFilesScanned(meta);
    }
}


void MediaInfo::scanFile(QString filename)
{
    if (minfo->isOpen()){
        qWarning() << "MediaInfo::scanFile\n\t" << "minfo is open!";
        minfo->kill();
    }

    currentFile = filename;
    minfo->start("mediainfo", QStringList(filename));
}


void MediaInfo::pringTags()
{
    QMultiMap<QString, QString> temp = metadata();

    temp.insert("ART", artFilePath);
    temp.insert("PLAYLISTART", playlistArtFilePath);
    temp.insert("MODIFIED", QString::number(QFileInfo(currentFile).lastModified().toMSecsSinceEpoch()));

    fixMetadata(currentFile, &temp);

    meta.insert(currentFile, temp);

    emit fileScanned();
}



static QRegExp rx_audio("^Audio");
static QRegExp rx_general("^General");
static QRegExp rx_format("^Format  .*: (.*)");
static QRegExp rx_duration("^Duration  .*: (.*)");
static QRegExp rx_bit_rate("^Overall bit rate  .*: (.*)");
static QRegExp rx_track_name("^Track name  .*: (.*)");
static QRegExp rx_sample_rate("^Sampling rate .*: (.*)");
static QRegExp rx_channels("^Channel.*: (.*) channels");
static QRegExp rx_artist("^Performer .*: (.*)");
static QRegExp rx_album("^Album .*: (.*)");
static QRegExp rx_date("^Recorded date .*: (.*)");
static QRegExp rx_genre("^Genre .*: (.*)");
static QRegExp rx_tracknumber("^Track name/Position .*: (.*)");
static QRegExp rx_album_artist("^ALBUM ARTIST .*: (.*)");



int MediaInfo::timeToSec(QString time)
{
    int sec=0;
    static QRegExp rx_hour("([0-9]*)h"), rx_min("([0-9]*)mn"), rx_sec("([0-9]*)s");

    if (rx_hour.indexIn(time) > -1){
        sec += rx_hour.cap(1).toInt() * 3600;
    }
    if (rx_min.indexIn(time) > -1){
        sec += rx_min.cap(1).toInt() * 60;
    }
    if (rx_sec.indexIn(time) > -1){
        sec += rx_sec.cap(1).toInt();
    }
    return sec;
}



QMultiMap<QString, QString> MediaInfo::metadata()
{
    QStringList out = QString::fromUtf8(minfo->readAllStandardOutput()).split("\n");

    minfo->close();

    QMultiMap<QString, QString> meta;

    foreach (QString line, out)
    {
        if (rx_format.indexIn(line) > -1){
            QString temp = rx_format.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("AUDIO-CODEC", temp);
        }

        else
        if (rx_duration.indexIn(line) > -1){
            QString temp = rx_duration.cap(1);
            temp = QTime(0, 0).addSecs(timeToSec(temp)).toString("mm:ss");
            meta.insert("DURATION", temp);
        }


        else
        if (rx_bit_rate.indexIn(line) > -1){
            QString temp = rx_bit_rate.cap(1);
            meta.insert("BITRATE", temp);
        }

        else
        if (rx_track_name.indexIn(line) > -1){
            QString temp = rx_track_name.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("TITLE", temp);
        }

        else
        if (rx_channels.indexIn(line) > -1){
            int ch = rx_channels.cap(1).toInt();
            QString temp;
            switch (ch) {
            case 1:
                temp = "Mono";
                break;
            case 2:
                temp = "Stereo";
                break;
            case 3:
                temp = "2.1";
                break;
            case 4:
                temp = "Quadro";
                break;
            case 5:
                temp = "5.0";
                break;
            case 6:
                temp = "5.1";
                break;
            case 7:
                temp = "6.1";
                break;
            case 8:
                temp = "7.1";
                break;
            default:
                qWarning() << "MediaInfo::parse: Unknown channels number \"" << ch << "\"";
            }
            meta.insert("CHANNEL-MODE", temp);
        }

        else
        if (rx_artist.indexIn(line) > -1){
            QString temp = rx_artist.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("ARTIST", temp);
        }

        if (rx_album_artist.indexIn(line) > -1){
            QString temp = rx_album_artist.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("ALBUM-ARTIST", temp);
        }

        else
        if (rx_album.indexIn(line) > -1){
            QString temp = rx_album.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("ALBUM", temp);
        }

        else
        if (rx_date.indexIn(line) > -1){
            QString temp = rx_date.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("DATE", temp);
        }

        else
        if (rx_genre.indexIn(line) > -1){
            QString temp = rx_genre.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("GENRE", temp);
        }

        else
        if (rx_tracknumber.indexIn(line) > -1){
            QString temp = rx_tracknumber.cap(1);

#ifdef Q_OS_WIN
            temp.remove(temp.size() - 1, 1);
#endif
            meta.insert("TRACK-NUMBER", temp);
        }
    }

    return meta;
}

