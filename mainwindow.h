#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QProcess>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QStatusBar>
#include <QRegExp>
#include <QLabel>
#include <QTableWidgetItem>
#include <QSlider>
#include <QToolButton>

#include "global.h"
#include "core.h"
#include "mediainfo.h"
#include "preferenceswindow.h"

using namespace Global;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString parseLine(MediaData *data, QString pattern);

//    QTableWidgetItem *playingItem, *playingItemLength;

    QSlider *progress;

    bool openNewTrack;
    bool readyToPlay;

public slots:
    void changeAlbumDir();

  //  void playFromPL(QModelIndex idx);
    void play();
    void stop();
    void playNext();
    void playPrev();

    void setTime(int seek);

    void plFilter();
    void directoryChanged(const QModelIndex &, const QModelIndex &);

    void setPlColumns(QStringList name = pref->pl_columns_names.split("[;]"),
                      QStringList size = pref->pl_columns_sizes.split(";"));
    void setPlRows(QStringList form = pref->pl_columns_format.split("[;]"));
    void highlightCurrentTrack(QStringList format = pref->pl_columns_playing_format.split("[;]"));

    void changePL(QStringList names, QStringList format, QStringList playformat, QStringList sizes);
    void resetPl();

    void showCurrentTime();
    void showDefTimePl();
    void defWindowTitle();
    void defPlhighlight();
    void tryFindCurrentTrack();

    void createToolBars();

private:
    Ui::MainWindow *ui;
    //void createColumns(const QString &pattern = pref->pl_columns);

    QFileSystemModel *FSmodel;
    QSortFilterProxyModel *sortProxy;
    QLabel *status;

    QString currentPath;
    QStringList PlPattern;

    void showPlPlaytime();


    int lengthColumn();
    int timeColumn;

protected:
    Core * core;
    MediaInfo * mediaInfo;
    PreferencesWindow * preferences;

};

#endif // MAINWINDOW_H
