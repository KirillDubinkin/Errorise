#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QProcess>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include "global.h"
#include "core.h"
#include "mediainfo.h"

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

public slots:
    void choseAlbumDir();
    void playFromPL(QModelIndex idx);
    void play(QString filename);
    void plFilter();
    void directoryChanged(const QModelIndex &, const QModelIndex &);


private:
    Ui::MainWindow *ui;
    void createColumns(const QString &pattern = pref->pl_columns);

    QFileSystemModel *FSmodel;
    QSortFilterProxyModel *sortProxy;

    QString currentPath;
    void changeDir(const QString fileName);
    void setPlColumns(const QString pattern = pref->pl_columns);



    QStringList filterFiles(const QDir &directory, const QStringList &files,
                                  const QString &text);
    void showFiles(const QDir &directory, const QStringList &files, const QString &pattern = pref->pl_columns);

protected:
    Core * core;
    MediaInfo * mediaInfo;
};

#endif // MAINWINDOW_H
