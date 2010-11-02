#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "core.h"

#include <QtDebug>
#include <QLabel>
#include <QTime>

using namespace Global;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    core = new Core(this);
    mediaInfo = new MediaInfo(this);
    preferences = new PreferencesWindow;

    FSmodel = new QFileSystemModel;

    FSmodel->setRootPath("");
    FSmodel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(FSmodel);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    changeAlbumDir();


    connect(ui->actionPreferences, SIGNAL(triggered()), this->preferences, SLOT(show()));
    connect(this->preferences, SIGNAL(music_folder_changed()), this, SLOT(changeAlbumDir()));
    connect(this->preferences, SIGNAL(file_filter_changed()), this, SLOT(plFilter()));
//    connect(this->preferences, SIGNAL(status_bar(bool)), this, SLOT(showStatusBar(bool)));
    connect(this->preferences, SIGNAL(hide_status_bar(bool)), ui->statusBar, SLOT(setHidden(bool)));
    connect(this->preferences, SIGNAL(playlist_changed(QStringList,QStringList,QStringList)),
                this, SLOT(changePL(QStringList,QStringList,QStringList)));
    connect(this->preferences, SIGNAL(playlist_reset()), this, SLOT(resetPl()));


    connect(this->core, SIGNAL(showTime(int)), this, SLOT(showCurrentTime(int)));


    connect(ui->actionChoose_Directory, SIGNAL(triggered()), this, SLOT(choseAlbumDir()));
    connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(play()));
    connect(ui->AlbumPL, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playFromPL(QModelIndex)));
    connect(ui->treeView->selectionModel(),
                    SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
                    SLOT(directoryChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(plFilter()));

   // connect(ui->AlbumPL, SIGNAL(pressed(QModelIndex)), this, SLOT(updateStatusBar(QModelIndex)));


    setPlColumns();
    ui->AlbumPL->hideColumn(0);

    ui->statusBar->setHidden(!pref->status_bar);
}

MainWindow::~MainWindow()
{
    delete FSmodel;
    delete mediaInfo;
    delete core;
    delete ui;
}

void MainWindow::showCurrentTime(int sec)
{
    if ( (!ui->statusBar->isHidden())
        && (pref->status_bar_format.contains("%playback_time%")) )
        {
        status->setText(parseLine(&core->mdat, pref->status_bar_format));
    }
}

void MainWindow::changePL(QStringList names, QStringList format, QStringList sizes)
{
    setPlColumns(names, sizes);
    setPlRows(format);
}

void MainWindow::resetPl()
{
    setPlColumns();
    setPlRows();
}

void MainWindow::setPlColumns(QStringList names, QStringList sizes)
{
    // first - absolete file path - hidden
    ui->AlbumPL->setColumnCount(1);
    int col=1;

    for (int i = 0; i < names.size(); i++)
    {
        ui->AlbumPL->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem(names.at(i));

        ui->AlbumPL->setHorizontalHeaderItem(col, item);
        ui->AlbumPL->setColumnWidth(col++, QString(sizes.at(i)).toInt());
    }
}


void MainWindow::setPlRows(QStringList form)
{
    int num = mediaInfo->numParsedFiles;

        // Sets new rowCount
    if (num  <=  ui->AlbumPL->rowCount()){
        ui->AlbumPL->setRowCount(num);
    } else {
        for (int i = ui->AlbumPL->rowCount(); i < num; i++){
            ui->AlbumPL->insertRow(i);
        }
    }

        // Adds absolete file path in first column
    for (int i = 0; i < num; i++){
        QTableWidgetItem *filepath = new QTableWidgetItem(mediaInfo->track[i].filename);
        ui->AlbumPL->setItem(i, 0, filepath);
    }

        // Fills remaining cells
    for (int i = 0, col = 1; i < form.size(); i++, col++){
        for (int row = 0; row < num; row++)
        {
            QLabel *label = new QLabel;
            label->setText( parseLine(&mediaInfo->track[row], form.at(i)) );
                // label gets parsed text

            ui->AlbumPL->setCellWidget(row, col, label);
        }
    }
}


QString MainWindow::parseLine(MediaData *data, QString pattern)
{
    pattern.replace("%title%", data->clip_name);
    pattern.replace("%artist%", data->clip_artist);
    pattern.replace("%album%", data->clip_album);
    pattern.replace("%date%", data->clip_date);
    pattern.replace("%genre%", data->clip_genre);
    pattern.replace("%tracknumber%", data->clip_track);
    pattern.replace("%codec%", data->audio_codec);
    pattern.replace("%format%", data->audio_format);
    pattern.replace("%bitrate%", data->bitrate);
    pattern.replace("%samplerate%", data->samplerate);
    pattern.replace("%channels%", data->channels);
    pattern.replace("%length%", MediaData::formatTime(data->duration));

    pattern.replace("%playback_time%", MediaData::formatTime(core->mset.current_sec));

    return pattern;
}



void MainWindow::changeAlbumDir()
{
    if (!pref->music_library_path.isEmpty()){

        ui->treeView->setRootIndex(FSmodel->index(pref->music_library_path));
        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);
    }
}

void MainWindow::directoryChanged(const QModelIndex &cur, const QModelIndex &)
{
    currentPath = FSmodel->filePath(cur);
}


void MainWindow::plFilter()
{
    QDir directory = QDir(currentPath);
    QStringList files;

    files = directory.entryList(pref->files_filter.split(";"),
                                        QDir::Files | QDir::NoSymLinks);

    mediaInfo->parseDir(directory.absolutePath(), files);
    //setPlRows(files);
    setPlRows();
}



void MainWindow::playFromPL(QModelIndex idx)
{
    core->mdat = mediaInfo->track[idx.row()];

    if (pref->play_only_this) {
      //  qDebug("Playing one track, and stop.");
        core->openFile(ui->AlbumPL->item(idx.row(), 0)->text());
    }
    else {
      //  qDebug("Playing many files:");
        QString files = ui->AlbumPL->item(idx.row(),0)->text();
        for (int row=idx.row()+1; row<ui->AlbumPL->rowCount(); row++){
            files += ";" + ui->AlbumPL->item(row,0)->text();
        }
        core->openFile(files);
    }
}

void MainWindow::play(QString filename)
{
    qDebug() << "Play: " << filename;

    core->openFile(filename);
}

