#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "core.h"
#include "version.h"

#include <QtDebug>
#include <QLabel>
#include <QTableWidgetItem>
#include <QGraphicsColorizeEffect>
#include <QColor>
#include <QPushButton>
#include <QSplitter>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QSpacerItem>




using namespace Global;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0, 0, pref->res_main_width, pref->res_main_height);
    defWindowTitle();

    core = new Core(this);
    mediaInfo = new MediaInfo(this);
    preferences = new PreferencesWindow;

    FSmodel = new QFileSystemModel;

    FSmodel->setRootPath("");
    FSmodel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(FSmodel);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    readyToPlay = false;


    timeColumn = -1;

    changeAlbumDir();
    createToolBars();


    connect(ui->actionPreferences, SIGNAL(triggered()), this->preferences, SLOT(show()));
    connect(this->preferences, SIGNAL(music_folder_changed()), this, SLOT(changeAlbumDir()));
    connect(this->preferences, SIGNAL(file_filter_changed()), this, SLOT(plFilter()));
//    connect(this->preferences, SIGNAL(status_bar(bool)), this, SLOT(showStatusBar(bool)));
    connect(this->preferences, SIGNAL(hide_status_bar(bool)), ui->statusBar, SLOT(setHidden(bool)));

    connect(this->preferences, SIGNAL(playlist_changed(QStringList,QStringList,QStringList,QStringList)),
            this, SLOT(changePL(QStringList,QStringList,QStringList,QStringList)));
    connect(this->preferences, SIGNAL(playlist_reset()), this, SLOT(resetPl()));


    connect(this->core, SIGNAL(showTime()), this, SLOT(showCurrentTime()));
    connect(this->preferences, SIGNAL(dontShowCurrentTimeInPl()), this, SLOT(showDefTimePl()));


    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));
    connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(playNext()));
    connect(ui->actionPrevios, SIGNAL(triggered()), this, SLOT(playPrev()));
    connect(ui->actionPause, SIGNAL(triggered()), this, SLOT(playPause()));


    connect(ui->AlbumPL, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(play()));
    connect(ui->treeView->selectionModel(),
                    SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
                    SLOT(directoryChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(plFilter()));

    connect(this->core, SIGNAL(finished()), status, SLOT(clear()));
    connect(this->core, SIGNAL(finished()), this, SLOT(defWindowTitle()));
//    connect(this->core, SIGNAL(finished()), this, SLOT(defPlhighlight()));
    connect(this->core, SIGNAL(playnext()), this, SLOT(playNext()));

   // connect(ui->AlbumPL, SIGNAL(pressed(QModelIndex)), this, SLOT(updateStatusBar(QModelIndex)));


    connect(this->progress, SIGNAL(sliderMoved(int)), this, SLOT(setTime(int)));
   // connect(this->progress, SIGNAL(actionTriggered(int)), this, SLOT(setTime(int)));

    connect(this->vol, SIGNAL(sliderMoved(int)), this, SLOT(setVol(int)));

    PlPattern = pref->pl_columns_format.split("[;]");


    setPlColumns();
    ui->AlbumPL->hideColumn(0);

    ui->statusBar->setHidden(!pref->status_bar);


    lengthColumn();


    //insertToolBars();

    ui->treeView->setGeometry(0,0,pref->res_tree_width, 0);


}

MainWindow::~MainWindow()
{
    pref->res_tree_width = ui->treeView->width();
    pref->res_main_width = this->width();
    pref->res_main_height = this->height();
    pref->res_pref_width = preferences->width();
    pref->res_pref_height = preferences->height();
    delete FSmodel;
    delete mediaInfo;
    delete core;
    delete ui;
}


void MainWindow::createToolBars()
{
    progress = new QSlider();
    progress->setOrientation(Qt::Horizontal);

    vol = new QSlider();
    vol->setOrientation(Qt::Horizontal);
    vol->setMaximumWidth(150);
    vol->setMinimum(0);
    vol->setMaximum(100);
    vol->setValue(pref->volume);

    QHBoxLayout *L = new QHBoxLayout(ui->menuBar);
    L->setSpacing(4);
    L->setMargin(1);

    L->addWidget(ui->menuMenu);
    L->addSpacing(25);
    L->addWidget(ui->mainToolBar);
    L->addWidget(progress);
    L->addWidget(vol);

}


void MainWindow::setVol(int vol)
{
    core->setVolume(vol);
}


void MainWindow::setTime(int seek)
{
    core->goToSec(seek);
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


void MainWindow::defPlhighlight()
{
    if (core->mset.current_id > -1){
        PlPattern = pref->pl_columns_format.split("[;]");
        for (int i = 0; i < PlPattern.size(); i++)
        {
            QLabel *label = new QLabel(parseLine(&core->mdat, PlPattern.at(i)));
            ui->AlbumPL->setCellWidget(core->mset.current_id, i+1, label);
        }
    }
}

void MainWindow::highlightCurrentTrack(QStringList format)
{
    if (core->mset.current_id == -1) // try to find
    {
        tryFindCurrentTrack();
    }

    QStringList back = pref->pl_columns_playng_back.split("[;]");

    if (core->mset.current_id > -1){
        for (int i = 0; i < format.size(); i++)
        {
            QLabel *label = new QLabel(parseLine(&core->mdat, "<b>" + format.at(i) + "</b>"));

            if (i+1 < back.size()){
                label->setStyleSheet("QLabel { " + back.at(i) + " }");
                qDebug() << "label->setStyle(" << i << "): " << back.at(i);
            }
//            label->setStyleSheet("QLabel { background-color: rgb(29, 66, 77); color: rgb(232, 232, 174) }");

            ui->AlbumPL->setCellWidget(core->mset.current_id, i+1, label);
        }
    }
}

void MainWindow::showCurrentTime()
{
    if (pref->window_title_format.contains("%playback_time%")){
        this->setWindowTitle(parseLine(&core->mdat, pref->window_title_format));
    }

    if ( (!ui->statusBar->isHidden())
        && (pref->status_bar_format.contains("%playback_time%")) )
        {
        status->setText(parseLine(&core->mdat, pref->status_bar_format));
    }

    if (pref->pl_show_playing_time){
        showPlPlaytime();
    }

    progress->setValue(core->mset.current_sec);
}


void MainWindow::showDefTimePl()
{
    if (lengthColumn() > 0)
    {
        QString temp;
        PlPattern = pref->pl_columns_format.split("[;]");

        temp = PlPattern.at(lengthColumn() -1);

        temp.replace("%length%", MediaData::formatTime(core->mdat.duration));

        QLabel *label = new QLabel(temp);
        ui->AlbumPL->setCellWidget(core->mset.current_id, lengthColumn(), label);
    }
}


void MainWindow::defWindowTitle()
{
    this->setWindowTitle("Errorise v." + amplayerVersion());
}


void MainWindow::tryFindCurrentTrack()
{
    int id = -2; // We try just once

    for (int i = 0; i < ui->AlbumPL->rowCount(); i++)
    {
        if (core->mdat.filename == ui->AlbumPL->item(i, 0)->text()){
            id = i;
        }
    }
    core->mset.current_id = id;
}

void MainWindow::showPlPlaytime()
{
    if (lengthColumn() > 0)
    {
        if (core->mset.current_id == -1) // try to find
        {
            tryFindCurrentTrack();
        }

        else if (core->mset.current_id > -1)
        {
            if (core->mdat.filename == ui->AlbumPL->item(core->mset.current_id, 0)->text())
            {
                QString temp;
                PlPattern = pref->pl_columns_format.split("[;]");

                temp = PlPattern.at(lengthColumn() -1);

                temp.replace("%length%", MediaData::formatTime(core->mset.current_sec));

                QLabel *label = new QLabel(temp);
               // qDebug() << label->text();
                ui->AlbumPL->setCellWidget(core->mset.current_id, lengthColumn(), label);
            }
            else { // Maybe, current track just go for a walk
                core->mset.current_id = -1;
            }
        }
    }
}

int MainWindow::lengthColumn()
{
    if (timeColumn < 1)
    {
        QString s;
        PlPattern = pref->pl_columns_format.split("[;]");
        for (int i = 0; i < PlPattern.size(); i++){
            s = PlPattern.at(i);
            if (s.contains("%length%")){
                timeColumn = i+1;
                return timeColumn;
            }
        }
        timeColumn = -1;
    }
    return timeColumn;
}


void MainWindow::changePL(QStringList names, QStringList format, QStringList playformat, QStringList sizes)
{
    setPlColumns(names, sizes);
    setPlRows(format);

    if (core->playing){
        highlightCurrentTrack(playformat);
    }
}

void MainWindow::resetPl()
{
    setPlColumns();
    setPlRows();

    if (core->playing){
        highlightCurrentTrack();
    }
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
    readyToPlay = false;
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

        readyToPlay = true;
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


    ui->AlbumPL->setCurrentCell(0,0);

    if (core->playing){
        core->mset.current_id = -1;
        // to check, if _realy_ current track present in playlist
        highlightCurrentTrack();
    }

}



void MainWindow::playNext()
{
    if (core->mset.current_id > -1){
        if (core->mset.current_id+1 < ui->AlbumPL->rowCount())
        {
            ui->AlbumPL->setCurrentCell( core->mset.current_id+1, 0);
            play();
        } else {
            defPlhighlight();
        }
    } else {
        if (ui->AlbumPL->currentRow() <= ui->AlbumPL->rowCount())
        {
            play();
        }
    }
}


void MainWindow::playPrev()
{
    if ( (core->playing) & (core->mset.current_id > 0) ){
        ui->AlbumPL->setCurrentCell( core->mset.current_id-1, 0 );
        play();
    }
}

void MainWindow::play()
{
    defPlhighlight();

    if (readyToPlay)
    {
        core->mdat = mediaInfo->track[ui->AlbumPL->currentRow()];
        core->mset.reset();
        core->mset.current_id = ui->AlbumPL->currentRow();
        core->restarting = true;

        core->openFile(ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text());

        this->setWindowTitle(parseLine(&core->mdat, pref->window_title_format));
        highlightCurrentTrack();

        /////////////////////
//        progress->setMinimum(0);

        progress->setMaximum(core->mdat.duration);
    }
}


void MainWindow::playPause()
{
    if (core->playing)
        core->pause();
    else
        play();
}

void MainWindow::stop()
{
    core->restarting=true;
    defPlhighlight();
    core->stop();
}
