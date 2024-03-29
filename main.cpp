#include <QApplication>
#include <QFile>
#include <QTime>
#include <QDir>
#include <QTextCodec>

#include "loader.h"
#include "global.h"

#include <stdio.h>

// #define NO_DEBUG_ON_CONSOLE


using namespace Global;

QFile output_log;

QTextCodec *localCodec;

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
                                fprintf( stderr, "Debug: %s\n",  localCodec->fromUnicode(orig_line).data() );
                                #endif
                                line2 = orig_line;
                        }
                        break;
                case QtWarningMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "WARNING: %s\n", localCodec->fromUnicode(orig_line).data() );
                        #endif
                        line2 = "WARNING: " + orig_line;
                        break;
                case QtFatalMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "FATAL: %s\n", localCodec->fromUnicode(orig_line).data() );
                        #endif
                        line2 = "FATAL: " + orig_line;
                        abort();                    // deliberately core dump
                case QtCriticalMsg:
                        #ifndef NO_DEBUG_ON_CONSOLE
                        fprintf( stderr, "CRITICAL: %s\n", localCodec->fromUnicode(orig_line).data() );
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
                            output_log.setFileName( pref->configPath() + "/" + qApp->applicationName() + ".log" );
                                output_log.open(QIODevice::Append);
                                output_log.write("################################################################################################\n"
                                                 "################################################################################################\n"
                                                 "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                                output_log.flush();
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
        a.setApplicationVersion(APP_VERSION);

        localCodec = QTextCodec::codecForLocale();

        QTextCodec *utf = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(utf);


        qInstallMsgHandler( myMessageOutput );

        Loader *loader = new Loader();
        loader->start();

        int r = a.exec();


        delete loader;
        if (output_log.isOpen()) output_log.close();
        return r;
}

