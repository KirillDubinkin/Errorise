#include "mediainfo.h"
#include "global.h"
#include <QStringList>
#include <QDebug>
#include <QRegExp>
#include <QTime>
#include <QFileInfo>

using namespace Global;

MediaInfo::MediaInfo(QObject *parent):
    QObject(parent)
{
    minfo = new QProcess;
    //track = new MediaData();
}


MediaInfo::~MediaInfo(){
    if (minfo->isOpen())
        minfo->terminate();
    delete minfo;
   // delete track;
}


void MediaInfo::start(QString file)
{
        if (minfo->isOpen()){
            minfo->waitForFinished();
        }


        minfo->start(pref->mediainfo_cli, QStringList() << file);
        minfo->waitForReadyRead();
        //qDebug() << minfo->readAllStandardOutput();
        out = QString(minfo->readAllStandardOutput()).split("\n");
        //qDebug() << out;
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

void MediaInfo::parseFile(QString file){
    if (!file.isEmpty()){
        start(file);
        QString line;

        for (int i = 2; i < out.size(); i++){
            line = out.at(i);

            if (rx_audio.indexIn(line) > -1){
                qDebug() << "rx_audio";
                return;
            }

            else
                if (rx_format.indexIn(line) > -1){
                //qDebug() <<  "rx_format:" << rx_format.cap(1);
                format = rx_format.cap(1);
                track[i].audio_format = rx_format.cap(1);
            }

            else
                if (rx_duration.indexIn(line) > -1){
                duration = rx_duration.cap(1);
                track[i].length = rx_duration.cap(1);

                qDebug() << QTime().fromString(rx_duration.cap(1), "m'm s's");
            }

            else
                if (rx_bit_rate.indexIn(line) > -1){
                bitrate = rx_bit_rate.cap(1);
            }

            else
                if (rx_track_name.indexIn(line) > -1){
                title = rx_track_name.cap(1);
            }

        }
    }
}


void MediaInfo::parseDir(const QString &dir, const QStringList &files)
{
    numParsedFiles = files.size();
    minfo->setWorkingDirectory(dir);
    minfo->start(pref->mediainfo_cli, files);
    //minfo->waitForReadyRead();
    minfo->waitForFinished();

    out = QString(minfo->readAllStandardOutput()).split("\n");

    //qDebug() << out.join("\n");
    parse(out, files);
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


void MediaInfo::parse(const QStringList &out, const QStringList &files)
{
    QString line;
    int id=-1;

    for (int i = 0; i < out.size(); i++)
    {
        line = out.at(i);

        if (rx_general.indexIn(line) > -1){
            id++;
            track[id].filename = minfo->workingDirectory() + "/" + files.at(id);
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
        }

        else
        if (rx_duration.indexIn(line) > -1){
            track[id].duration = timeToSec(rx_duration.cap(1));
             //   qDebug() << QTime().fromString(rx_duration.cap(1), "m'm s's");
        }

        else
        if (rx_bit_rate.indexIn(line) > -1){
            track[id].bitrate = rx_bit_rate.cap(1);
            //qDebug() << "Bit rate: " << track[id].audio_bitrate;
        }

        else
        if (rx_track_name.indexIn(line) > -1){
            track[id].clip_name = rx_track_name.cap(1);
        }

        else
        if (rx_sample_rate.indexIn(line) > -1){
            track[id].samplerate = rx_sample_rate.cap(1);
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
        }

        else
        if (rx_album.indexIn(line) > -1){
            track[id].clip_album = rx_album.cap(1);
        }

        else
        if (rx_date.indexIn(line) > -1){
            track[id].clip_date = rx_date.cap(1);
        }

        else
        if (rx_genre.indexIn(line) > -1){
            track[id].clip_genre = rx_genre.cap(1);
        }

        else
        if (rx_tracknumber.indexIn(line) > -1){
            track[id].clip_track = rx_tracknumber.cap(1);
        }
    }
}
