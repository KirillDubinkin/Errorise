#include "helper.h"
#include "global.h"
#include <QFileInfo>
#include <QDebug>

using namespace Global;

QString Helper::parseLine(const int GUID, QString pattern) const
{
    pattern.replace("%title%", mediainfo->track[GUID].clip_name);
    pattern.replace("%artist%", mediainfo->track[GUID].clip_artist);
    pattern.replace("%album_artist%", mediainfo->track[GUID].album_artist);
    pattern.replace("%album%", mediainfo->track[GUID].clip_album);
    pattern.replace("%date%", mediainfo->track[GUID].clip_date);
    pattern.replace("%genre%", mediainfo->track[GUID].clip_genre);
    pattern.replace("%tracknumber%", mediainfo->track[GUID].clip_track);
    pattern.replace("%codec%", mediainfo->track[GUID].audio_codec);
    pattern.replace("%format%", mediainfo->track[GUID].audio_format);
    pattern.replace("%bitrate%", mediainfo->track[GUID].bitrate);
    pattern.replace("%samplerate%", mediainfo->track[GUID].samplerate);
    pattern.replace("%channels%", mediainfo->track[GUID].channels);
    pattern.replace("%length%", formatTime(mediainfo->track[GUID].duration));

//    pattern.replace("%playback_time%", MediaData::formatTime(core->mset.current_sec));

 //   qDebug() << pattern;

    return pattern;
}

QString Helper::formatTime(int sec) const
{
    int hours = (int) sec / 3600;
    sec -= hours*3600;
    int minutes = (int) sec / 60;
    sec -= minutes*60;
    int seconds = sec;

    QString tf;
    return tf.sprintf("%02d:%02d:%02d",hours,minutes,seconds);
}

QString Helper::filePath(const int GUID)
{
    if (mediainfo->numParsedFiles >= GUID)
        return mediainfo->track[GUID].filename;

    return NULL;
}


int Helper::guidOf(const QString &filename)
{
    for(int i = 0; i < mediainfo->numParsedFiles; i++)
    {
        if (mediainfo->track[i].filename == filename)
            return i;
    }

    return -1;
}
