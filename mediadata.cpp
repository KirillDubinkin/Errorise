/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2010 Ricardo Villalba <rvm@escomposlinux.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "mediadata.h"
#include <QFileInfo>
#include <cmath>


MediaData::MediaData() {
	reset();
}

MediaData::~MediaData() {
}

void MediaData::reset() {
	filename="";
	type = TYPE_UNKNOWN;
	duration=0;

	audios.clear();

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


void MediaData::list() {
	qDebug("MediaData::list");

	qDebug("  filename: '%s'", filename.toUtf8().data());
	qDebug("  duration: %f", duration);

	qDebug("  type: %d", type);
	qDebug("  initialized: %d", initialized);

        qDebug("  Subs:");

        qDebug("  Videos:");
	qDebug("  Audios:");
	audios.list();

        qDebug("  Titles:");

        qDebug("  demuxer: '%s'", demuxer.toUtf8().data() );
        qDebug("  audio_format: '%s'", audio_format.toUtf8().data() );
	qDebug("  audio_bitrate: %d", audio_bitrate );
	qDebug("  audio_rate: %d", audio_rate );
        qDebug("  audio_nch: %d", audio_nch );
	qDebug("  audio_codec: '%s'", audio_codec.toUtf8().data() );
}

