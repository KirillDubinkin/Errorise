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

    QRegExp rx_var;
    /*
    QRegExp rx_codec;
    QRegExp rx_bitrate;
    QRegExp rx_samplerate;
    QRegExp rx_bits;
    QRegExp rx_channels;
    QRegExp rx_playback_time;
    QRegExp rx_length;
    */

    QString parseLine(const int &idx, QString pattern);


public slots:
    void changeAlbumDir();
    void playFromPL(QModelIndex idx);
    void play(QString filename);
    void plFilter();
    void directoryChanged(const QModelIndex &, const QModelIndex &);
    void updateStatusBar(const QModelIndex &idx);


private:
    Ui::MainWindow *ui;
    void createColumns(const QString &pattern = pref->pl_columns);

    QFileSystemModel *FSmodel;
    QSortFilterProxyModel *sortProxy;

    QString currentPath;
    void setPlColumns(const QString pattern = pref->pl_columns);

    QStringList filterFiles(const QDir &directory, const QStringList &files,
                                  const QString &text);
    void showFiles(const QDir &directory, const QStringList &files, const QString &pattern = pref->pl_columns);

protected:
    Core * core;
    MediaInfo * mediaInfo;
    PreferencesWindow * preferences;
};

#endif // MAINWINDOW_H
