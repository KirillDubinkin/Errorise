#include "mediadata.h"
#include <QFileInfo>
#include <cmath>


MediaData::MediaData() {
	reset();
}

MediaData::~MediaData() {
}

void MediaData::reset() {

    filename = "";

    type = TYPE_UNKNOWN;
    duration=0;

    initialized=false;

	// Clip info;
    clip_name = "";
    clip_artist = "";
    clip_author = "";
    clip_album = "";
    clip_genre = "";
    clip_date = "";
    clip_track = "";
    clip_copyright = "";
    clip_comment = "";
    clip_software = "";

	stream_title = "";
	stream_url = "";

	// Other data
        demuxer="";
        audio_format="";
	audio_bitrate=0;
	audio_rate=0;
        audio_nch=0;
	audio_codec="";
}

QString MediaData::displayName() {
	if (!clip_name.isEmpty()) return clip_name;
	else
	if (!stream_title.isEmpty()) return stream_title;

	QFileInfo fi(filename);
	if (fi.exists()) 
		return fi.fileName(); // filename without path
	else
		return filename;
}


QString MediaData::formatTime(int secs) {
    int t = secs;
    int hours = (int) t / 3600;
    t -= hours*3600;
    int minutes = (int) t / 60;
    t -= minutes*60;
    int seconds = t;

    QString tf;
    return tf.sprintf("%02d:%02d:%02d",hours,minutes,seconds);
}


void MediaData::list() {
	qDebug("MediaData::list");

	qDebug("  filename: '%s'", filename.toUtf8().data());
	qDebug("  duration: %f", duration);

	qDebug("  type: %d", type);
	qDebug("  initialized: %d", initialized);

        qDebug("  Subs:");

        qDebug("  Videos:");
	qDebug("  Audios:");

        qDebug("  Titles:");

        qDebug("  demuxer: '%s'", demuxer.toUtf8().data() );
        qDebug("  audio_format: '%s'", audio_format.toUtf8().data() );
	qDebug("  audio_bitrate: %d", audio_bitrate );
	qDebug("  audio_rate: %d", audio_rate );
        qDebug("  audio_nch: %d", audio_nch );
	qDebug("  audio_codec: '%s'", audio_codec.toUtf8().data() );
}

