#include "amplayer.h"
#include "version.h"
#include "global.h"
#include "translator.h"
#include "preferences.h"

#include <stdio.h>
#include <QApplication>

using namespace Global;

AMPlayer::AMPlayer(QObject * parent ) :
    QObject(parent)
{
    //main_window = 0;
    s_gui = 0;
    //gui_to_use = "DefaultGui";

    close_at_end = -1; // Not set

    move_gui = false;
    resize_gui = false;

    global_init();

    // Application translations
//    translator->load( pref->language );
    showInfo();
}

AMPlayer::~AMPlayer()
{
    //if (main_window != 0) delete main_window;
    if (s_gui != 0) delete s_gui;
    global_end();
}





SimpleGUI * AMPlayer::sgui()
{
    if (s_gui == 0){
        s_gui = new SimpleGUI(0);
    }
    return s_gui;
}

void AMPlayer::start() {
        sgui()->show();
}



void AMPlayer::showInfo() {
#ifdef Q_OS_WIN
        QString win_ver;
        switch (QSysInfo::WindowsVersion) {
                case QSysInfo::WV_32s: win_ver = "Windows 3.1"; break;
                case QSysInfo::WV_95: win_ver = "Windows 95"; break;
                case QSysInfo::WV_98: win_ver = "Windows 98"; break;
                case QSysInfo::WV_Me: win_ver = "Windows Me"; break;
                case QSysInfo::WV_NT: win_ver = "Windows NT"; break;
                case QSysInfo::WV_2000: win_ver = "Windows 2000"; break;
                case QSysInfo::WV_XP: win_ver = "Windows XP"; break;
                case QSysInfo::WV_2003: win_ver = "Windows Server 2003"; break;
                case QSysInfo::WV_VISTA: win_ver = "Windows Vista"; break;
                #if QT_VERSION >= 0x040501
                case QSysInfo::WV_WINDOWS7: win_ver = "Windows 7"; break;
                #endif
                default: win_ver = QString("other: %1").arg(QSysInfo::WindowsVersion);
        }
#endif
        QString s = QObject::tr("This is %1 v. %2 running on %3").arg(myplayerName())
                    .arg(myplayerVersion())
#ifdef Q_OS_LINUX
           .arg("Linux")
#else
#ifdef Q_OS_WIN
           .arg("Windows ("+win_ver+")")
#else
                   .arg("Other OS")
#endif
#endif
           ;

        printf("%s\n", s.toLocal8Bit().data() );
        qDebug("%s", s.toUtf8().data() );
        qDebug("Compiled with Qt v. %s, using %s", QT_VERSION_STR, qVersion());

        qDebug(" * application path: '%s'", qApp->applicationDirPath().toUtf8().data());
        qDebug(" * config path: '%s'", pref->configPath().toUtf8().data());
        qDebug(" * current path: '%s'", QDir::currentPath().toUtf8().data());
}
