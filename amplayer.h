#ifndef AMPLAYER_H
#define AMPLAYER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "mainwindow.h"

class AMPlayer : public QObject
{
   // Q_OBJECT
public:
        enum ExitCode { ErrorArgument = -3, NoAction = -2, NoRunningInstance = -1, NoError = 0, NoExit = 1 };

        AMPlayer(const QString & config_path = QString::null, QObject * parent = 0);
        ~AMPlayer();

        //! Process arguments. If ExitCode != NoExit the application must be exited.
//        ExitCode processArgs(QStringList args);

        MainWindow * gui();

        void start();

private:
#ifndef PORTABLE_APP
        void createConfigDirectory();
#endif
        void showInfo();

        MainWindow * main_window;

        QStringList files_to_play;
        QString actions_list; //!< Actions to be run on startup
        //QString gui_to_use;

        // Change position and size
        bool move_gui;
        QPoint gui_position;

        bool resize_gui;
        QSize gui_size;

        // Options to pass to gui
        int close_at_end; // -1 = not set, 1 = true, 0 false
};


#endif // AMPLAYER_H
