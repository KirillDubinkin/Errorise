#include "core.h"

#include <QFileInfo>
#include <QDebug>

#include "mplayerprocess.h"
#include "mplayerversion.h"
#include "mediasettings.h"
#include "mediadata.h"
#include "preferences.h"
#include "global.h"
#include "filters.h"

using namespace Global;

Core::Core(QObject *parent) :
    QObject(parent)
{
    proc = new MplayerProcess(this);
    mset.reset();
    playing = false;
    restarting = false;

    connect(proc, SIGNAL(receivedCSec(int)), this, SLOT(changeCurrentSec(int)));
    connect(proc, SIGNAL(processExited()), this, SLOT(processFinished()));

}

Core::~Core()
{
    //qDebug() << "Core::~Core()";
    restarting = true;
    if (proc->isRunning()) stopMplayer();
    proc->terminate();
    delete proc;
}

void Core::stop()
{
    this->stopMplayer();
}



void Core::setVolume(int volume) {
        qDebug("Core::setVolume: %d", volume);

        int current_volume = (pref->volume);

        if (volume == current_volume) return;

        current_volume = volume;
        if (current_volume > 100) current_volume = 100;
        if (current_volume < 0) current_volume = 0;
/*
        if (state() == Paused) {
                // Change volume later, after quiting pause
                change_volume_after_unpause = true;
        } else */{
                tellmp("volume " + QString::number(current_volume) + " 1");
        }


        pref->volume = current_volume;
        pref->mute = false;

        //updateWidgets();

        //displayMessage( tr("Volume: %1").arg(current_volume) );
        //emit volumeChanged( current_volume );
}



void Core::goToSec( int sec ) {
        qDebug("Core::goToSec: %d", sec);

    if (sec < 0) sec = 0;
    if (sec > mdat.duration ) sec = mdat.duration - 1;
    tellmp("seek " + QString::number(sec) + " 2");
}


void Core::processFinished()
{
    emit finished();
    if ((!restarting) & (!pref->play_only_this)){
        emit playnext();
    }
}

void Core::changeCurrentSec(int sec)
{
    static int tsec;
    if (tsec != sec){
        tsec = sec;

        mset.current_sec = sec;

        emit showTime();

      //  qDebug() << sec;
    }
}

void Core::openFile(QString filename, int seek) {
        //qDebug("Core::openFile: '%s'", filename.toUtf8().data());

    if (pref->play_only_this){
        QFileInfo fi(filename);
        if (fi.exists()) {
                playNewFile(fi.absoluteFilePath(), seek);
        } else {
            qWarning("Core::openFile: FILE DOESN'T EXIST!");
        }
    } else {
        playNewFile(filename, seek);
    }
}



void Core::playNewFile(QString file, int seek) {
        //qDebug("Core::playNewFile: '%s'", file.toUtf8().data());

        if (proc->isRunning()) {
                stopMplayer();
        }

 //       mdat.reset();
 //       mdat.filename = file;
 //       mdat.type = TYPE_FILE;

//        int old_volume = mset.volume;
      //  mset.reset();
//        mset.volume = old_volume;

        /* initializeMenus(); */

//        qDebug("Core::playNewFile: volume: %d, old_volume: %d", mset.volume, old_volume);
        initPlaying(seek);
}




void Core::initPlaying(int seek) {
        qDebug("Core::initPlaying");

        if (proc->isRunning()) {
                stopMplayer();
        }

        int start_sec = mset.current_sec;
        if (seek > -1) start_sec = seek;

        startMplayer( mdat.filename, start_sec );
}





void Core::startMplayer(QString file, double seek ) {
        qDebug("Core::startMplayer");

        if (file.isEmpty()) {
                qWarning("Core:startMplayer: file is empty!");
                return;
        }

        if (proc->isRunning()) {
                qWarning("Core::startMplayer: MPlayer still running!");
                return;
        }


//        bool is_mkv = (QFileInfo(file).suffix().toLower() == "mkv");

        // URL
//	bool url_is_playlist = file.endsWith(IS_PLAYLIST_TAG);
//	if (url_is_playlist) file = file.remove( QRegExp(IS_PLAYLIST_TAG_RX) );

        proc->clearArguments();



        // Use absolute path, otherwise after changing to the screenshot directory
        // the mplayer path might not be found if it's a relative path
        // (seems to be necessary only for linux)
        QString mplayer_bin = pref->mplayer_bin;
/*	QFileInfo fi(mplayer_bin);
        if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
            mplayer_bin = fi.absoluteFilePath();
        }
*/
        proc->addArgument( mplayer_bin );

        proc->addArgument("-noquiet");

        if (pref->verbose_log) {
                proc->addArgument("-v");
        }

        // Demuxer and audio and video codecs:
/*	if (!mset.forced_demuxer.isEmpty()) {
                proc->addArgument("-demuxer");
                proc->addArgument(mset.forced_demuxer);
        }
*/	if (!mset.forced_audio_codec.isEmpty()) {
                proc->addArgument("-ac");
                proc->addArgument(mset.forced_audio_codec);
        }

        if (pref->use_hwac3) {
                proc->addArgument("-afm");
                proc->addArgument("hwac3");
        }


        proc->addArgument("-identify");

                // We need this to get info about mkv chapters
  /*              if (is_mkv) {
                        proc->addArgument("-msglevel");
                        proc->addArgument("demux=6");

                        // **** Reset chapter ***
                        // Select first chapter, otherwise we cannot
                        // resume playback at the same point
                        // (time would be relative to chapter)
                        mset.current_chapter_id = 0;
                }
*/

        proc->addArgument("-novideo");

        proc->addArgument("-slave");


        if (!pref->ao.isEmpty()) {
                proc->addArgument( "-ao");
                proc->addArgument( pref->ao );
        }




        // Performance options
        #ifdef Q_OS_WIN
        QString p;
        int app_p = NORMAL_PRIORITY_CLASS;
        switch (pref->priority) {
                case Preferences::Realtime: 	p = "realtime";
                                                                                app_p = REALTIME_PRIORITY_CLASS;
                                                                                break;
                case Preferences::High:			p = "high";
                                                                                app_p = REALTIME_PRIORITY_CLASS;
                                                                                break;
                case Preferences::AboveNormal:	p = "abovenormal";
                                                                                app_p = HIGH_PRIORITY_CLASS;
                                                                                break;
                case Preferences::Normal: 		p = "normal";
                                                                                app_p = ABOVE_NORMAL_PRIORITY_CLASS;
                                                                                break;
                case Preferences::BelowNormal: 	p = "belownormal"; break;
                case Preferences::Idle: 		p = "idle"; break;
                default: 						p = "normal";
        }
        proc->addArgument("-priority");
        proc->addArgument( p );
        SetPriorityClass(GetCurrentProcess(), app_p);
        qDebug("Core::startMplayer: priority of smplayer process set to %d", app_p);
        #endif


        proc->addArgument("-volume");
        proc->addArgument( QString::number( pref->volume ) );

        proc->addArgument("-nocache");

        // Enable the OSD later, to avoid a lot of messages to be
        // printed on startup
        proc->addArgument("-osdlevel");
        proc->addArgument( "0" );


/*        if (mdat.type == TYPE_STREAM) {
                if (pref->prefer_ipv4) {
                        proc->addArgument("-prefer-ipv4");
                } else {
                        proc->addArgument("-prefer-ipv6");
                }
        }
*/

        // Audio channels
        if (mset.audio_use_channels != 0) {
                proc->addArgument("-channels");
                proc->addArgument( QString::number( mset.audio_use_channels ) );
        }

        // Audio filters
        QString af="";
        if (mset.karaoke_filter) {
                af="karaoke";
        }

        // Stereo mode
        if (mset.stereo_mode != 0) {
                if (mset.stereo_mode == MediaSettings::Left)
                        af += "channels=2:2:0:1:0:0";
                else
                        af += "channels=2:2:1:0:1:1";
        }

        if (mset.extrastereo_filter) {
                if (!af.isEmpty()) af += ",";
                af += "extrastereo";
        }

        if (mset.volnorm_filter) {
                if (!af.isEmpty()) af += ",";
                af += pref->filters->item("volnorm").filter();
        }


/*        // Audio equalizer
        if (pref->use_audio_equalizer) {
                if (!af.isEmpty()) af += ",";
                af += "equalizer=" + Helper::equalizerListToString(mset.audio_equalizer);
        }
*/

        // Additional audio filters, supplied by user
        // File
        if ( !pref->mplayer_additional_audio_filters.isEmpty() ) {
                if (!af.isEmpty()) af += ",";
                af += pref->mplayer_additional_audio_filters;
        }
        // Global
        if ( !mset.mplayer_additional_audio_filters.isEmpty() ) {
                if (!af.isEmpty()) af += ",";
                af += mset.mplayer_additional_audio_filters;
        }

        if (!af.isEmpty()) {
                // Don't use audio filters if using the S/PDIF output
                if (pref->use_hwac3) {
                        qDebug("Core::startMplayer: audio filters are disabled when using the S/PDIF output!");
                } else {
                        proc->addArgument("-af");
                        proc->addArgument( af );
                }
        }

        if (pref->use_soft_vol) {
                proc->addArgument("-softvol");
                proc->addArgument("-softvol-max");
                proc->addArgument( QString::number(pref->softvol_max) );
        }



        // Additional options supplied by the user
        // File
        if (!mset.mplayer_additional_options.isEmpty()) {
                QStringList args = MyProcess::splitArguments(mset.mplayer_additional_options);
        QStringList::Iterator it = args.begin();
        while( it != args.end() ) {
                        proc->addArgument( (*it) );
                        ++it;
                }
        }
        // Global
        if (!pref->mplayer_additional_options.isEmpty()) {
                QStringList args = MyProcess::splitArguments(pref->mplayer_additional_options);
        QStringList::Iterator it = args.begin();
        while( it != args.end() ) {
                        proc->addArgument( (*it) );
                        ++it;
                }
        }

        // File to play


#ifdef Q_OS_WIN
        if (pref->use_short_pathnames)
                proc->addArgument(Helper::shortPathName(file));
        else
#endif

        if (pref->play_only_this){
            proc->addArgument(file);
        } else {
            QStringList list = file.split(";");
            for (int i=0; i < list.size(); i++){
                qDebug() << "FILE: " << list.at(i);
                proc->addArgument(list.at(i));
            }
        }


        // It seems the loop option must be after the filename
        if (mset.loop) {
                proc->addArgument("-loop");
                proc->addArgument("0");
        }

//        emit aboutToStartPlaying();

        QString commandline = proc->arguments().join(" ");
        qDebug("Core::startMplayer: command: '%s'", commandline.toUtf8().data());

        //Log command
        QString line_for_log = commandline + "\n";
//        emit logLineAvailable(line_for_log);

        playing = true;

        if ( !proc->start() ) {
            // error handling
            playing = false;
            qWarning("Core::startMplayer: mplayer process didn't start");
        } else {
            restarting = false;
        }

}

void Core::pause()
{
    if (proc->isRunning())
        if (this->playing)
            this->tellmp("pause");
}


void Core::stopMplayer() {
        qDebug("Core::stopMplayer");

        if (!proc->isRunning()) {
                qWarning("Core::stopMplayer: mplayer in not running!");
                return;
        }

    tellmp("quit");

        qDebug("Core::stopMplayer: Waiting mplayer to finish...");
        if (!proc->waitForFinished(5000)) {
                qWarning("Core::stopMplayer: process didn't finish. Killing it...");
                proc->kill();
        }

        qDebug("Core::stopMplayer: Finished. (I hope)");
}


void Core::tellmp(const QString & command) {
        qDebug("Core::tellmp: '%s'", command.toUtf8().data());

    //qDebug("Command: '%s'", command.toUtf8().data());
    if (proc->isRunning()) {
                proc->writeToStdin( command );
    } else {
                qWarning(" tellmp: no process running: %s", command.toUtf8().data());
    }
}

