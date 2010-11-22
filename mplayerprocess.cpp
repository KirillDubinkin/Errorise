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

#include "mplayerprocess.h"
#include <QRegExp>
#include <QStringList>
#include <QApplication>

#include "global.h"
#include "preferences.h"
#include "mplayerversion.h"
#include "colorutils.h"

using namespace Global;

MplayerProcess::MplayerProcess(QObject * parent) : MyProcess(parent) 
{

#if NOTIFY_AUDIO_CHANGES
	qRegisterMetaType<Tracks>("Tracks");
#endif

	connect( this, SIGNAL(lineAvailable(QByteArray)),
			 this, SLOT(parseLine(QByteArray)) );

	connect( this, SIGNAL(finished(int,QProcess::ExitStatus)), 
             this, SLOT(processFinished(int,QProcess::ExitStatus)) );

	connect( this, SIGNAL(error(QProcess::ProcessError)),
             this, SLOT(gotError(QProcess::ProcessError)) );

	notified_mplayer_is_running = false;

	mplayer_svn = -1; // Not found yet
}

MplayerProcess::~MplayerProcess() {
}

bool MplayerProcess::start() {
	md.reset();
	notified_mplayer_is_running = false;
	mplayer_svn = -1; // Not found yet
	received_end_of_file = false;

#if NOTIFY_AUDIO_CHANGES
	audios.clear();
	audio_info_changed = false;
#endif

	MyProcess::start();
	return waitForStarted();
}

void MplayerProcess::writeToStdin(QString text) {
	if (isRunning()) {
		//qDebug("MplayerProcess::writeToStdin");
		write( text.toLocal8Bit() + "\n");
	} else {
		qWarning("MplayerProcess::writeToStdin: process not running");
	}
}

static QRegExp rx_av("^[AV]: *([0-9,:.-]+)");

static QRegExp rx_frame("^[AV]:.* (\\d+)\\/.\\d+");// [0-9,.]+");
static QRegExp rx("^(.*)=(.*)");
#if !NOTIFY_AUDIO_CHANGES
static QRegExp rx_audio_mat("^ID_AID_(\\d+)_(LANG|NAME)=(.*)");
#endif

static QRegExp rx_ao("^AO: \\[(.*)\\]");
static QRegExp rx_paused("^ID_PAUSED");

static QRegExp rx_cache("^Cache fill:.*");
static QRegExp rx_create_index("^Generating Index:.*");
static QRegExp rx_play("^Starting playback...");
static QRegExp rx_connecting("^Connecting to .*");
static QRegExp rx_resolving("^Resolving .*");

static QRegExp rx_endoffile("^Exiting... \\(End of file\\)|^ID_EXIT=EOF");
static QRegExp rx_mkvchapters("\\[mkv\\] Chapter (\\d+) from");




// Audio CD
static QRegExp rx_cdda("^ID_CDDA_TRACK_(\\d+)_MSF=(.*)");


// Audio
#if NOTIFY_AUDIO_CHANGES
static QRegExp rx_audio("^ID_AUDIO_ID=(\\d+)");
static QRegExp rx_audio_info("^ID_AID_(\\d+)_(LANG|NAME)=(.*)");
#endif


//Clip info
static QRegExp rx_clip_name("^ (name|title): (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_artist("^ artist: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_author("^ author: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_album("^ album: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_genre("^ genre: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_date("^ (creation date|year): (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_track("^ track: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_copyright("^ copyright: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_comment("^ comment: (.*)", Qt::CaseInsensitive);
static QRegExp rx_clip_software("^ software: (.*)", Qt::CaseInsensitive);

static QRegExp rx_stream_title("^.* StreamTitle='(.*)';");
static QRegExp rx_stream_title_and_url("^.* StreamTitle='(.*)';StreamUrl='(.*)';");


void MplayerProcess::parseLine(QByteArray ba) {
	//qDebug("MplayerProcess::parseLine: '%s'", ba.data() );

	QString tag;
	QString value;

#if COLOR_OUTPUT_SUPPORT
    QString line = ColorUtils::stripColorsTags(QString::fromLocal8Bit(ba));
#else
	QString line = QString::fromLocal8Bit(ba);
#endif

	// Parse A: V: line
	//qDebug("%s", line.toUtf8().data());
    if (rx_av.indexIn(line) > -1) {
		double sec = rx_av.cap(1).toDouble();

                if ( rx_av.cap(1).contains(".0") ){
                    emit receivedCSec(sec);
                }

		//qDebug("cap(1): '%s'", rx_av.cap(1).toUtf8().data() );
		//qDebug("sec: %f", sec);

#if NOTIFY_AUDIO_CHANGES
		if (notified_mplayer_is_running) {
			if (audio_info_changed) {
				qDebug("MplayerProcess::parseLine: audio_info_changed");
				audio_info_changed = false;
				emit audioInfoChanged(audios);
			}
		}
#endif

		if (!notified_mplayer_is_running) {
			qDebug("MplayerProcess::parseLine: starting sec: %f", sec);

			emit receivedStartingTime(sec);
			emit mplayerFullyLoaded();

			emit receivedCurrentFrame(0); // Ugly hack: set the frame counter to 0

			notified_mplayer_is_running = true;
		}
		
	    emit receivedCurrentSec( sec );
	}
	else {
		// Emulates mplayer version in Ubuntu:
		//if (line.startsWith("MPlayer 1.0rc1")) line = "MPlayer 2:1.0~rc1-0ubuntu13.1 (C) 2000-2006 MPlayer Team";

		// Emulates unknown version
		//if (line.startsWith("MPlayer SVN")) line = "MPlayer lalksklsjjakksja";

		emit lineAvailable(line);

		// Parse other things
		qDebug("MplayerProcess::parseLine: '%s'", line.toUtf8().data() );

		// End of file
		if (rx_endoffile.indexIn(line) > -1)  {
			qDebug("MplayerProcess::parseLine: detected end of file");
			if (!received_end_of_file) {
				// In case of playing VCDs or DVDs, maybe the first title
    	        // is not playable, so the GUI doesn't get the info about
        	    // available titles. So if we received the end of file
            	// first let's pretend the file has started so the GUI can have
	            // the data.
				if ( !notified_mplayer_is_running) {
					emit mplayerFullyLoaded();
				}

				//emit receivedEndOfFile();
				// Send signal once the process is finished, not now!
				received_end_of_file = true;
			}
		}
		else

		// Pause
		if (rx_paused.indexIn(line) > -1) {
			emit receivedPause();
		}

		// Stream title
		if (rx_stream_title_and_url.indexIn(line) > -1) {
			QString s = rx_stream_title_and_url.cap(1);
			QString url = rx_stream_title_and_url.cap(2);
			qDebug("MplayerProcess::parseLine: stream_title: '%s'", s.toUtf8().data());
			qDebug("MplayerProcess::parseLine: stream_url: '%s'", url.toUtf8().data());
			md.stream_title = s;
			md.stream_url = url;
			emit receivedStreamTitleAndUrl( s, url );
		}
		else
		if (rx_stream_title.indexIn(line) > -1) {
			QString s = rx_stream_title.cap(1);
			qDebug("MplayerProcess::parseLine: stream_title: '%s'", s.toUtf8().data());
			md.stream_title = s;
			emit receivedStreamTitle( s );
		}


#if NOTIFY_AUDIO_CHANGES
		// Audio
		if (rx_audio.indexIn(line) > -1) {
			int ID = rx_audio.cap(1).toInt();
			qDebug("MplayerProcess::parseLine: ID_AUDIO_ID: %d", ID);
			if (audios.find(ID) == -1) audio_info_changed = true;
			audios.addID( ID );
		}

		if (rx_audio_info.indexIn(line) > -1) {
			int ID = rx_audio_info.cap(1).toInt();
			QString lang = rx_audio_info.cap(3);
			QString t = rx_audio_info.cap(2);
			qDebug("MplayerProcess::parseLine: Audio: ID: %d, Lang: '%s' Type: '%s'", 
                    ID, lang.toUtf8().data(), t.toUtf8().data());

			int idx = audios.find(ID);
			if (idx == -1) {
				qDebug("MplayerProcess::parseLine: audio %d doesn't exist, adding it", ID);

				audio_info_changed = true;
				if ( t == "NAME" ) 
					audios.addName(ID, lang);
//				else
//					audios.addLang(ID, lang);
			} else {
				qDebug("MplayerProcess::parseLine: audio %d exists, modifing it", ID);

				if (t == "NAME") {
					//qDebug("MplayerProcess::parseLine: name of audio %d: %s", ID, audios.itemAt(idx).name().toUtf8().constData());
					if (audios.itemAt(idx).name() != lang) {
						audio_info_changed = true;
						audios.addName(ID, lang);
					}
                                }
			}
		}
#endif


		// The following things are not sent when the file has started to play
		// (or if sent, smplayer will ignore anyway...)
		// So not process anymore, if video is playing to save some time
		if (notified_mplayer_is_running) {
			return;
		}

		if ( (mplayer_svn == -1) && (line.startsWith("MPlayer ")) ) {
			mplayer_svn = MplayerVersion::mplayerVersion(line);
			qDebug("MplayerProcess::parseLine: MPlayer SVN: %d", mplayer_svn);
			if (mplayer_svn <= 0) {
				qWarning("MplayerProcess::parseLine: couldn't parse mplayer version!");
				emit failedToParseMplayerVersion(line);
			}
		}


		// AO
		if (rx_ao.indexIn(line) > -1) {
			emit receivedAO( rx_ao.cap(1) );
		}
		else

#if !NOTIFY_AUDIO_CHANGES
		// Matroska audio
		if (rx_audio_mat.indexIn(line) > -1) {
			int ID = rx_audio_mat.cap(1).toInt();
			QString lang = rx_audio_mat.cap(3);
			QString t = rx_audio_mat.cap(2);
			qDebug("MplayerProcess::parseLine: Audio: ID: %d, Lang: '%s' Type: '%s'", 
                    ID, lang.toUtf8().data(), t.toUtf8().data());

			if ( t == "NAME" ) 
				md.audios.addName(ID, lang);
			else
				md.audios.addLang(ID, lang);
		}
		else
#endif



                // Audio CD titles
		if (rx_cdda.indexIn(line) > -1 ) {
//			int ID = rx_cdda.cap(1).toInt();
			QString length = rx_cdda.cap(2);
			double duration = 0;
			QRegExp r("(\\d+):(\\d+):(\\d+)");
			if ( r.indexIn(length) > -1 ) {
				duration = r.cap(1).toInt() * 60;
				duration += r.cap(2).toInt();
			}
//                        md.titles.addID( ID );

                        //QString name = QString::number(ID) + " (" + length + ")";
                        //md.titles.addName( ID, name );

//                        md.titles.addDuration( ID, duration );
                }
		else


		// Catch cache messages
		if (rx_cache.indexIn(line) > -1) {
			emit receivedCacheMessage(line);
		}
		else

		// Creating index
		if (rx_create_index.indexIn(line) > -1) {
			emit receivedCreatingIndex(line);
		}
		else

		// Catch connecting message
		if (rx_connecting.indexIn(line) > -1) {
			emit receivedConnectingToMessage(line);
		}
		else

		// Catch resolving message
		if (rx_resolving.indexIn(line) > -1) {
			emit receivedResolvingMessage(line);
		}
		else


		// Clip info

		//QString::trimmed() is used for removing leading and trailing whitespaces
		//Some .mp3 files contain tags with starting and ending whitespaces
		//Unfortunately MPlayer gives us leading and trailing whitespaces, Winamp for example doesn't show them

		// Name
		if (rx_clip_name.indexIn(line) > -1) {
			QString s = rx_clip_name.cap(2).trimmed();
			qDebug("MplayerProcess::parseLine: clip_name: '%s'", s.toUtf8().data());
			md.clip_name = s;
		}
		else

		// Artist
		if (rx_clip_artist.indexIn(line) > -1) {
			QString s = rx_clip_artist.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_artist: '%s'", s.toUtf8().data());
			md.clip_artist = s;
		}
		else

		// Author
		if (rx_clip_author.indexIn(line) > -1) {
			QString s = rx_clip_author.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_author: '%s'", s.toUtf8().data());
			md.clip_author = s;
		}
		else

		// Album
		if (rx_clip_album.indexIn(line) > -1) {
			QString s = rx_clip_album.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_album: '%s'", s.toUtf8().data());
			md.clip_album = s;
		}
		else

		// Genre
		if (rx_clip_genre.indexIn(line) > -1) {
			QString s = rx_clip_genre.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_genre: '%s'", s.toUtf8().data());
			md.clip_genre = s;
		}
		else

		// Date
		if (rx_clip_date.indexIn(line) > -1) {
			QString s = rx_clip_date.cap(2).trimmed();
			qDebug("MplayerProcess::parseLine: clip_date: '%s'", s.toUtf8().data());
			md.clip_date = s;
		}
		else

		// Track
		if (rx_clip_track.indexIn(line) > -1) {
			QString s = rx_clip_track.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_track: '%s'", s.toUtf8().data());
			md.clip_track = s;
		}
		else

		// Copyright
		if (rx_clip_copyright.indexIn(line) > -1) {
			QString s = rx_clip_copyright.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_copyright: '%s'", s.toUtf8().data());
			md.clip_copyright = s;
		}
		else

		// Comment
		if (rx_clip_comment.indexIn(line) > -1) {
			QString s = rx_clip_comment.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_comment: '%s'", s.toUtf8().data());
			md.clip_comment = s;
		}
		else

		// Software
		if (rx_clip_software.indexIn(line) > -1) {
			QString s = rx_clip_software.cap(1).trimmed();
			qDebug("MplayerProcess::parseLine: clip_software: '%s'", s.toUtf8().data());
			md.clip_software = s;
		}
		else


		// Catch starting message
		/*
		pos = rx_play.indexIn(line);
		if (pos > -1) {
			emit mplayerFullyLoaded();
		}
		*/

                //Generic things
                if (rx.indexIn(line) > -1) {
			tag = rx.cap(1);
			value = rx.cap(2);
			//qDebug("MplayerProcess::parseLine: tag: %s, value: %s", tag.toUtf8().data(), value.toUtf8().data());

#if !NOTIFY_AUDIO_CHANGES
			// Generic audio
			if (tag == "ID_AUDIO_ID") {
				int ID = value.toInt();
				qDebug("MplayerProcess::parseLine: ID_AUDIO_ID: %d", ID);
				md.audios.addID( ID );
			}
			else
#endif

			if (tag == "ID_LENGTH") {
				md.duration = value.toDouble();
				qDebug("MplayerProcess::parseLine: md.duration set to %f", md.duration);
                        }
			else
			if (tag == "ID_DEMUXER") {
				md.demuxer = value;
                        }
			else
			if (tag == "ID_AUDIO_FORMAT") {
				md.audio_format = value;
                        }
			else
			if (tag == "ID_AUDIO_BITRATE") {
				md.audio_bitrate = value.toInt();
			}
			else
			if (tag == "ID_AUDIO_RATE") {
				md.audio_rate = value.toInt();
			}
			else
			if (tag == "ID_AUDIO_NCH") {
				md.audio_nch = value.toInt();
                        }
			else
			if (tag == "ID_AUDIO_CODEC") {
				md.audio_codec = value;
			}
                        /*
#if GENERIC_CHAPTER_SUPPORT
			else
			if (tag == "ID_CHAPTERS") {
				md.chapters = value.toInt();
			}
			else
			if (tag == "ID_DVD_CURRENT_TITLE") {
				dvd_current_title = value.toInt();
			}
#endif */
                }
	}
}

// Called when the process is finished
void MplayerProcess::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
	qDebug("MplayerProcess::processFinished: exitCode: %d, status: %d", exitCode, (int) exitStatus);
	// Send this signal before the endoffile one, otherwise
	// the playlist will start to play next file before all
	// objects are notified that the process has exited.
	emit processExited();
	if (received_end_of_file) emit receivedEndOfFile();
}

void MplayerProcess::gotError(QProcess::ProcessError error) {
	qDebug("MplayerProcess::gotError: %d", (int) error);
}

#include "moc_mplayerprocess.cpp"
