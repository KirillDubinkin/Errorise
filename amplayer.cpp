#include "amplayer.h"
#include "paths.h"
#include "version.h"
#include "global.h"
#include "translator.h"
#include "preferences.h"

#include <stdio.h>
#include <QApplication>

using namespace Global;

AMPlayer::AMPlayer(const QString & config_path, QObject * parent ) :
    QObject(parent)
{
    main_window = 0;
    //gui_to_use = "DefaultGui";

    close_at_end = -1; // Not set

    move_gui = false;
    resize_gui = false;

    Paths::setAppPath( qApp->applicationDirPath() );

#ifndef PORTABLE_APP
    if (config_path.isEmpty()) createConfigDirectory();
#endif
    global_init(config_path);

    // Application translations
    translator->load( pref->language );
    showInfo();
}

AMPlayer::~AMPlayer()
{
    if (main_window != 0) delete main_window;
    global_end();
}



#ifndef PORTABLE_APP
void AMPlayer::createConfigDirectory() {
        if (!QFile::exists(Paths::configPath())) {
                QDir d;
                if (!d.mkdir(Paths::configPath())) {
                        qWarning("AMPlayer::createConfigDirectory: can't create %s", Paths::configPath().toUtf8().data());
                }
        }
}
#endif



MainWindow * AMPlayer::gui() {
        if (main_window == 0) {
                // Changes to app path, so smplayer can find a relative mplayer path
                QDir::setCurrent(Paths::appPath());
                qDebug("AMPlayer::gui: changed working directory to app path");
                qDebug("AMPlayer::gui: current directory: %s", QDir::currentPath().toUtf8().data());

                /*if (gui_to_use.toLower() == "minigui")
                        main_window = new MiniGui(0);
                else
                if (gui_to_use.toLower() == "mpcgui")
                        main_window = new MpcGui(0);
                else
                        main_window = new DefaultGui(0);
*/
                main_window = new MainWindow(0);

                if (move_gui) {
                        qDebug("AMPlayer::gui: moving main window to %d %d", gui_position.x(), gui_position.y());
                        main_window->move(gui_position);
                }
                if (resize_gui) {
                        qDebug("AMPlayer::gui: resizing main window to %dx%d", gui_size.width(), gui_size.height());
                        main_window->resize(gui_size);
                }

                //main_window->setForceCloseOnFinish(close_at_end);
        }

        return main_window;
}



void AMPlayer::start() {
        //if (!gui()->startHidden() || !files_to_play.isEmpty() ) gui()->show();
        gui()->show();
        /*if (!files_to_play.isEmpty()) {
                if (!subtitle_file.isEmpty()) gui()->setInitialSubtitle(subtitle_file);
                gui()->openFiles(files_to_play);
        }

        if (!actions_list.isEmpty()) {
                if (files_to_play.isEmpty()) {
                        gui()->runActions(actions_list);
                } else {
                        gui()->runActionsLater(actions_list);
                }
        }*/
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
        QString s = QObject::tr("This is AMPlayer v. %1 running on %2")
                    .arg(smplayerVersion())
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

        qDebug(" * application path: '%s'", Paths::appPath().toUtf8().data());
        qDebug(" * data path: '%s'", Paths::dataPath().toUtf8().data());
        qDebug(" * translation path: '%s'", Paths::translationPath().toUtf8().data());
        qDebug(" * doc path: '%s'", Paths::docPath().toUtf8().data());
        qDebug(" * themes path: '%s'", Paths::themesPath().toUtf8().data());
        qDebug(" * shortcuts path: '%s'", Paths::shortcutsPath().toUtf8().data());
        qDebug(" * config path: '%s'", Paths::configPath().toUtf8().data());
        qDebug(" * ini path: '%s'", Paths::iniPath().toUtf8().data());
        qDebug(" * current path: '%s'", QDir::currentPath().toUtf8().data());
}
