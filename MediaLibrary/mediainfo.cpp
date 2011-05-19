#include "mediainfo.h"

#include <QDebug>
#include <QRegExp>
#include <QTime>
#include <QFileInfo>


MediaInfo::MediaInfo(QObject *parent): Minfo(parent)
{
    minfo = new QProcess(this);

    connect(minfo, SIGNAL(finished(int)), this, SLOT(pringTags()));
    connect(this, SIGNAL(fileScanned()), this, SLOT(scanNextFile()));
}


void MediaInfo::scanFiles(QStringList files)
{
    isUpdateState = false;

    qDebug() << "MediaInfo::scanFiles" << files.size();

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

    qDebug() << "MediaInfo::reScanFiles" << files.size();

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

    QStringList format = currentFile.split(".");
    temp.insert("FORMAT", QString(format.last()).toUpper());

    temp.insert("ART", artFilePath);
    temp.insert("PLAYLISTART", playlistArtFilePath);
    temp.insert("MODIFIED", QString::number(QFileInfo(currentFile).lastModified().toMSecsSinceEpoch()));

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
    QStringList out = QString::fromLocal8Bit(minfo->readAllStandardOutput()).split("\n");
    minfo->close();

    QMultiMap<QString, QString> meta;

    foreach (QString line, out)
    {
/*        if (rx_general.indexIn(line) > -1){
            track[id].filename = files.at(id);
            track[id].audio_codec = QFileInfo(track[id].filename).suffix().toUpper();
           // qDebug() << "filename" << track[id].filename;
        }

        else */

/*        if (rx_audio.indexIn(line) > -1){
             //   qDebug() << "rx_audio";
            //break;
        }

        else */
        if (rx_format.indexIn(line) > -1){
            QString temp = rx_format.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("AUDIO-CODEC", temp);
        }

/*        else
        if (rx_duration.indexIn(line) > -1){
            track[id].duration = timeToSec(rx_duration.cap(1));
             //   qDebug() << QTime().fromString(rx_duration.cap(1), "m'm s's");
        }


        else
        if (rx_bit_rate.indexIn(line) > -1){
            track[id].bitrate = rx_bit_rate.cap(1);
#ifdef Q_OS_WIN
            track[id].bitrate.remove(track[id].bitrate.size()-1, 1);
#endif
            //qDebug() << "Bit rate: " << track[id].audio_bitrate;
        }
*/
        else
        if (rx_track_name.indexIn(line) > -1){
            QString temp = rx_track_name.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("TITLE", temp);
        }
/*
        else
        if (rx_sample_rate.indexIn(line) > -1){
            track[id].samplerate = rx_sample_rate.cap(1);
#ifdef Q_OS_WIN
            track[id].samplerate.remove(track[id].samplerate.size()-1, 1);
#endif
        }

        else
        if (rx_channels.indexIn(line) > -1){
            track[id].audio_nch = rx_channels.cap(1).toInt();
            switch (track[id].audio_nch){
            case 1:
                track[id].channels = "Mono";
                break;
            case 2:
                track[id].channels = "Stereo";
                break;
            case 3:
                track[id].channels = "2.1";
                break;
            case 4:
                track[id].channels = "Quadro";
                break;
            case 5:
                track[id].channels = "5.0";
                break;
            case 6:
                track[id].channels = "5.1";
                break;
            case 7:
                track[id].channels = "6.1";
                break;
            case 8:
                track[id].channels = "8.1";
                break;
            default:
                qWarning() << "MediaInfo::parse: Unknown channels number \"" << track[id].audio_nch << "\"";
            }


        }
*/
        else
        if (rx_artist.indexIn(line) > -1){
            QString temp = rx_artist.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("ARTIST", temp);
        }

        if (rx_album_artist.indexIn(line) > -1){
            QString temp = rx_album_artist.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("ALBUM-ARTIST", temp);
        }

        else
        if (rx_album.indexIn(line) > -1){
            QString temp = rx_album.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("ALBUM", temp);
        }

        else
        if (rx_date.indexIn(line) > -1){
            QString temp = rx_date.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("DATE", temp);
        }
/*
        else
        if (rx_genre.indexIn(line) > -1){
            track[id].clip_genre = rx_genre.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_genre.remove(track[id].clip_genre.size()-1, 1);
#endif
        }
*/
        else
        if (rx_tracknumber.indexIn(line) > -1){
            QString temp = rx_date.cap(1);
            temp.remove(temp.size() - 1, 1);
            meta.insert("TRACK-NUMBER", temp);
        }
    }

    return meta;
}

