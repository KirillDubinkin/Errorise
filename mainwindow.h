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

    QSlider *progress, *vol;

    bool dont_change_time; // KOSTbIL' ^^
    bool openNewTrack;
    bool readyToPlay;

public slots:
    void changeAlbumDir();

  //  void playFromPL(QModelIndex idx);
    void play();
    void stop();
    void playNext();
    void playPrev();
    void playPause();

    void setTime(int seek);
    void setVol(int vol);

    void plFilter();
    void directoryChanged(const QModelIndex &, const QModelIndex &);

    void setPlColumns();
    void setPlRows();
    void setPlGroupRows();
    void highlightCurrentTrack();

    void changePL();

    void showCurrentTime();
    void showDefTimePl();
    void defWindowTitle();
    void defPlhighlight();
    void tryFindCurrentTrack();

    void createToolBars();

    void setColors();

private:
    Ui::MainWindow *ui;
    //void createColumns(const QString &pattern = pref->pl_columns);

    QFileSystemModel *FSmodel;
    QSortFilterProxyModel *sortProxy;
    QLabel *status;

    QString currentPath;
    QStringList files;
    QStringList PlPattern;

    void showPlPlaytime();

    QTableWidgetItem *newItem(const QBrush &background, Qt::ItemFlags flags = Qt::ItemIsEnabled,  const QString &text = "");

    void addRowItem(int idx, int row, int col);
    void addRowLabel(int idx, int row, int col);

    void addGroupItem(int row, const QString &text);
    void addGroupLabel(int row, const QString &text);

    void addCover(int row, int spanRow, const QDir &path);

    void recursiveDirectory(const QString &sDir);

    int lengthColumn();
    int timeColumn;
    int coverColumn;

protected:
    Core * core;
    MediaInfo * mediaInfo;
    PreferencesWindow * preferences;

};

#endif // MAINWINDOW_H
