#include <QApplication>
#include <QFile>
#include <QTime>
#include <QDir>

#include "amplayer.h"
#include "version.h"
#include "global.h"
#include "paths.h"

#include <stdio.h>

// #define NO_DEBUG_ON_CONSOLE


using namespace Global;

//MainWindow * mainwindow_instance = 0;
QFile output_log;

void myMessageOutput( QtMsgType type, const char *msg )
{
        static QString orig_line;
        static QString line2;
        static QRegExp rx_log;

        if (pref) {
                if (!pref->log) return;
                rx_log.setPattern(pref->log_filter);
        } else {
                rx_log.setPattern(".*");
        }

        line2.clear();

        orig_line = QString::fromUtf8(msg);

        switch ( type ) {
                case QtDebugMsg:
                        if (rx_log.indexIn(orig_line) > -1) {
                                #ifndef NO_DEBUG_ON_CONSOLE
                                fprintf( stderr, "Debug: %s\n", orig_line.toLocal8Bit().data() );
                                #endif
                                line2 = orig_line;
                        }
                        break;
                case QtWarningMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "WARNING: %s\n", orig_line.toLocal8Bit().data() );
                        #endif
                        line2 = "WARNING: " + orig_line;
                        break;
                case QtFatalMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "FATAL: %s\n", orig_line.toLocal8Bit().data() );
                        #endif
                        line2 = "FATAL: " + orig_line;
                        abort();                    // deliberately core dump
                case QtCriticalMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "CRITICAL: %s\n", orig_line.toLocal8Bit().data() );
                        #endif
                        line2 = "CRITICAL: " + orig_line;
                        break;
        }

        if (line2.isEmpty()) return;

        line2 = "["+ QTime::currentTime().toString("hh:mm:ss:zzz") +"] "+ line2;


        if (pref) {
                if (pref->save_log) {
                        // Save log to file
                        if (!output_log.isOpen()) {
                            output_log.setFileName( Paths::configPath() + QDir::separator() + myplayerName().toLower() + "_log.txt" );
                                output_log.open(QIODevice::WriteOnly);
                        }
                        if (output_log.isOpen()) {
                                QString l = line2 + "\r\n";
                                output_log.write(l.toUtf8().constData());
                                output_log.flush();
                        }
                }
        }
}



int main( int argc, char ** argv )
{
        QApplication a( argc, argv );
        a.setQuitOnLastWindowClosed(true);
        a.setApplicationName("Errorise");
        //a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

        // Sets the config path
        QString config_path;
/*
#ifdef PORTABLE_APP
        config_path = a.applicationDirPath();
#else
        // If a myplayerName.ini exists in the app path, will use that path
        // for the config file by default
        if (QFile::exists( a.applicationDirPath() + QDir::separator() + myplayerName().toLower() + ".ini" ) ) {
                config_path = a.applicationDirPath();
                qDebug("main: using existing %s", QString(config_path + QDir::separator() + myplayerName().toLower() + ".ini").toUtf8().data());
        }
#endif
*/
        //QStringList args = a.arguments();

        qInstallMsgHandler( myMessageOutput );

        AMPlayer * amplayer = new AMPlayer(config_path);
/*        AMPlayer::ExitCode c = amplayer->processArgs( args );
        if (c != AMPlayer::NoExit) {
                return c;
        }
*/
       // mainwindow_instance = amplayer->sgui();
        //a.connect(amplayer->gui(), SIGNAL(quitSolicited()), &a, SLOT(quit()));
        amplayer->start();

        int r = a.exec();

        //mainwindow_instance = 0;
        delete amplayer;

        if (output_log.isOpen()) output_log.close();

        return r;
}

