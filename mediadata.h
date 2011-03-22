#ifndef _MEDIADATA_H_
#define _MEDIADATA_H_

/* Here we store some volatile info about the file we need to remember */

//#include "tracks.h"
//  #include "subtracks.h"
//  #include "titletracks.h"
//#include "config.h"

#include <QString>
#include <QSettings>


// Types of media

#define TYPE_UNKNOWN -1
#define TYPE_FILE 0
//#define TYPE_DVD 1
#define TYPE_STREAM 2
//#define TYPE_VCD 3
//#define TYPE_AUDIO_CD 4
//#define TYPE_TV 5

class MediaData {

public:
	MediaData();
	virtual ~MediaData();

	virtual void reset();
        static QString formatTime(int secs);

	QString filename;
	double duration;
        QString length;

        int type; // file, dvd...
	bool initialized;

	void list();

       // Tracks audios;

	// Clip info
        QString album_artist;

	QString clip_name;
	QString clip_artist;
	QString clip_author;
	QString clip_album;
	QString clip_genre;
	QString clip_date;
	QString clip_track;
	QString clip_copyright;
	QString clip_comment;
	QString clip_software;

	QString stream_title;
	QString stream_url;


	// Other data not really useful for us,
	// just to show info to the user.
        QString bitrate;
        QString samplerate;
        QString channels;
        QString demuxer;
        QString audio_format;  // like a "Digital Theater System"
	int audio_bitrate;
	int audio_rate;
	int audio_nch; // channels?
        QString audio_codec;   // like a "DTS"

	/*QString info();*/
	QString displayName();
};

#endif
