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

#include <QDebug>

#include "global.h"
#include "preferences.h"
#include "mplayerversion.h"
#include "colorutils.h"

using namespace Global;


MplayerProcess::MplayerProcess(QObject * parent) : MyProcess(parent) 
{

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
        //md.reset();
	notified_mplayer_is_running = false;
	mplayer_svn = -1; // Not found yet
	received_end_of_file = false;

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



void MplayerProcess::parseLine(QByteArray ba) {

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

                emit receivedCSec(sec);


		//qDebug("cap(1): '%s'", rx_av.cap(1).toUtf8().data() );
		//qDebug("sec: %f", sec);


		if (!notified_mplayer_is_running) {
                        //qDebug("MplayerProcess::parseLine: starting sec: %f", sec);

			emit receivedStartingTime(sec);
			emit mplayerFullyLoaded();

                        //emit receivedCurrentFrame(0); // Ugly hack: set the frame counter to 0

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
                //qDebug("MplayerProcess::parseLine: '%s'", line.toUtf8().data() );

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
        qWarning("MplayerProcess::gotError: %d", (int) error);
}

#include "moc_mplayerprocess.cpp"
