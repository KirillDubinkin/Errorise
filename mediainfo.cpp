#include "mediainfo.h"
#include "global.h"
#include <QStringList>
#include <QDebug>
#include <QRegExp>
#include <QTime>

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
    minfo->setWorkingDirectory(dir);
    minfo->start(pref->mediainfo_cli, files);
    minfo->waitForReadyRead();

    out = QString(minfo->readAllStandardOutput()).split("\n");

    //qDebug() << out.join("\n");
    parse(out, files);
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
            track[id].length = rx_duration.cap(1);
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


    }
}
