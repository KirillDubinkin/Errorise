#include "mediainfo.h"
#include "global.h"
#include <QStringList>
#include <QDebug>
#include <QRegExp>
#include <QTime>
#include <QFileInfo>
#include <QDir>
#include <QTimer>

using namespace Global;

MediaInfo::MediaInfo(QObject *parent):
    QObject(parent)
{
    track = 0;
    minfo = new QProcess(this);
    restarting = false;

    connect(this, SIGNAL(filesGetted()), this, SLOT(startScan()));

    connect(minfo, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(readStdOut(int,QProcess::ExitStatus)));

    connect(this, SIGNAL(stdoutParsed()), this, SLOT(takeGuids()));
}


MediaInfo::~MediaInfo(){
    if (minfo->isOpen())
        minfo->terminate();

    if (track)
        delete [] track;
}


void MediaInfo::getFiles()
{
    recursiveDirs(path);

    if (files.isEmpty())
        return;

    emit filesGetted();
}


void MediaInfo::recursiveDirs(const QString &sDir)
{
    QDir dir(sDir);
    QFileInfoList list = dir.entryInfoList(pref->files_filter.split(";"), QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];

        QString sFilePath = info.filePath();
        if (info.isDir())
        {
            // recursive
            recursiveDirs(sFilePath);
        }
        else
        {
            files << sFilePath;
        }
    }
}



void MediaInfo::scanDir(const QString &spath)
{
    files.clear();
    path = spath;

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

    if (pref->recursive_dirs)
        QTimer::singleShot(0, this, SLOT(getFiles()));
    else
    {
        files = QDir(path).entryList(pref->files_filter.split(";"),
                                            QDir::Files);
        if (files.isEmpty())
            return;

        emit filesGetted();
    }
}


void MediaInfo::startScan()
{
    if (minfo->isOpen())
    {
        restarting = true;
        minfo->close();
    }
#ifdef Q_OS_WIN
    out.clear();

    restarting = false;
    for (int i = 0; i < files.size(); i++)
    {
        minfo->start(pref->mediainfo_cli, files);
        minfo->waitForFinished();
        out += QString::fromLocal8Bit(minfo->readAllStandardOutput()).split("\n");
    }

#else
    minfo->start(pref->mediainfo_cli, files);
    restarting = false;
        //! And we waiting 'closed' signal...
#endif
}


#ifdef Q_OS_WIN
void MediaInfo::readStdOut(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        numParsedFiles = files.size();

        if (track)
            delete [] track;
        track = new MediaData[numParsedFiles];

        QTimer::singleShot(0, this, SLOT(parceStdOut()));
    }

    if (!restarting)
        qWarning() << "MediaInfo crashed:\n" << exitStatus << endl;
}
#else


void MediaInfo::readStdOut(int, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        numParsedFiles = files.size();

        if (track)
            delete [] track;
        track = new MediaData[numParsedFiles];

        out = QString::fromLocal8Bit(minfo->readAllStandardOutput()).split("\n");

        return QTimer::singleShot(0, this, SLOT(parceStdOut()));
    }

    if (!restarting)
        qWarning() << "MediaInfo crashed:\n" << exitStatus << endl;
}
#endif


void MediaInfo::takeGuids()
{
    guid.clear();

    for (int i = 0; i < numParsedFiles; i++)
        guid.append(i);

    emit newTracksReceived(guid);

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif
}


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


void MediaInfo::parceStdOut()
{
    QString line;
    int id=-1;

    for (int i = 0; i < out.size(); i++)
    {
        line = out.at(i);

        if (rx_general.indexIn(line) > -1){
            id++;
            track[id].filename = files.at(id);
            track[id].audio_codec = QFileInfo(track[id].filename).suffix().toUpper();
           // qDebug() << "filename" << track[id].filename;
        }

        else
        if (rx_audio.indexIn(line) > -1){
             //   qDebug() << "rx_audio";
            //break;
        }

        else
        if (rx_format.indexIn(line) > -1){
            track[id].audio_format = rx_format.cap(1);
#ifdef Q_OS_WIN
            track[id].audio_format.remove(track[id].audio_format.size(), 1);
#endif
        }

        else
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

        else
        if (rx_track_name.indexIn(line) > -1){
            track[id].clip_name = rx_track_name.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_name.remove(track[id].clip_name.size()-1, 1);
#endif
        }

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

        else
        if (rx_artist.indexIn(line) > -1){
            track[id].clip_artist = rx_artist.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_artist.remove(track[id].clip_artist.size()-1, 1);
#endif
            track[id].album_artist = track[id].clip_artist;
        }

        if (rx_album_artist.indexIn(line) > -1){
            track[id].album_artist = rx_album_artist.cap(1);
#ifdef Q_OS_WIN
            track[id].album_artist.remove(track[id].album_artist.size()-1, 1);
#endif
        }

        else
        if (rx_album.indexIn(line) > -1){
            track[id].clip_album = rx_album.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_album.remove(track[id].clip_album.size()-1, 1);
#endif
        }

        else
        if (rx_date.indexIn(line) > -1){
            track[id].clip_date = rx_date.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_date.remove(track[id].clip_date.size()-1, 1);
#endif
        }

        else
        if (rx_genre.indexIn(line) > -1){
            track[id].clip_genre = rx_genre.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_genre.remove(track[id].clip_genre.size()-1, 1);
#endif
        }

        else
        if (rx_tracknumber.indexIn(line) > -1){
            track[id].clip_track = rx_tracknumber.cap(1);
#ifdef Q_OS_WIN
            track[id].clip_track.remove(track[id].clip_track.size()-1, 1);
#endif
        }
    }

    emit stdoutParsed();
}
