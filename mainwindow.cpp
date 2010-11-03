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

using namespace Global;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    timeColumn = -1;

    changeAlbumDir();


    connect(ui->actionPreferences, SIGNAL(triggered()), this->preferences, SLOT(show()));
    connect(this->preferences, SIGNAL(music_folder_changed()), this, SLOT(changeAlbumDir()));
    connect(this->preferences, SIGNAL(file_filter_changed()), this, SLOT(plFilter()));
//    connect(this->preferences, SIGNAL(status_bar(bool)), this, SLOT(showStatusBar(bool)));
    connect(this->preferences, SIGNAL(hide_status_bar(bool)), ui->statusBar, SLOT(setHidden(bool)));
    connect(this->preferences, SIGNAL(playlist_changed(QStringList,QStringList,QStringList)),
                this, SLOT(changePL(QStringList,QStringList,QStringList)));
    connect(this->preferences, SIGNAL(playlist_reset()), this, SLOT(resetPl()));


    connect(this->core, SIGNAL(showTime()), this, SLOT(showCurrentTime()));
    connect(this->preferences, SIGNAL(dontShowCurrentTimeInPl()), this, SLOT(showDefTimePl()));


    connect(ui->actionChoose_Directory, SIGNAL(triggered()), this, SLOT(choseAlbumDir()));
    connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(play()));
    connect(ui->AlbumPL, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playFromPL(QModelIndex)));
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


    PlPattern = pref->pl_columns_format.split("[;]");


    setPlColumns();
    ui->AlbumPL->hideColumn(0);

    ui->statusBar->setHidden(!pref->status_bar);


    lengthColumn();

}

MainWindow::~MainWindow()
{
    delete FSmodel;
    delete mediaInfo;
    delete core;
    delete ui;
}


void MainWindow::defPlhighlight()
{
    if (core->mset.current_id > -1){
        for (int i = 0; i < PlPattern.size(); i++)
        {
            QLabel *label = new QLabel(parseLine(&core->mdat, PlPattern.at(i)));
            ui->AlbumPL->setCellWidget(core->mset.current_id, i+1, label);
        }
    }
}

void MainWindow::highlightCurrentTrack()
{
    int id = -1;
    for (int i = 0; i < ui->AlbumPL->rowCount(); i++)
    {
        if (core->mdat.filename == ui->AlbumPL->item(i, 0)->text()){
            id = i;
        }
    }

    qDebug() << "id: " << id;

    if (id > -1){
        for (int i = 0; i < PlPattern.size(); i++)
        {
            QLabel *label = new QLabel(parseLine(&core->mdat, "<b>" + PlPattern.at(i) + "</b>"));
         //   QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
         //   effect->setColor(QColor(192,128,50));

         //   label->setGraphicsEffect(effect);

            label->setStyleSheet("QLabel { background-color: rgb(29, 66, 77); color: rgb(232, 232, 174) }");

            ui->AlbumPL->setCellWidget(id, i+1, label);
        }

        core->mset.current_id = id;
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
}


void MainWindow::showDefTimePl()
{
    if (lengthColumn() > 0)
    {
        QString temp;
        temp = PlPattern.at(lengthColumn() -1);

        temp.replace("%length%", MediaData::formatTime(core->mdat.duration));

        QLabel *label = new QLabel(temp);
        ui->AlbumPL->setCellWidget(core->mset.current_id, lengthColumn(), label);
    }
}


void MainWindow::defWindowTitle()
{
    this->setWindowTitle("AMPlayer v." + amplayerVersion());
}


void MainWindow::tryFindCurrentTrack()
{
    core->mset.current_id = -2;  // We try just once
    for (int i = 0; i < ui->AlbumPL->rowCount(); i++)
    {

        if (core->mdat.filename == ui->AlbumPL->item(i, 0)->text()){
            core->mset.current_id = i;
        }
    }
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

    if (core->playing){
        core->mset.current_id = -1;
        // to check, if _realy_ current track present in playlist
    }
    highlightCurrentTrack();
}



void MainWindow::playFromPL(QModelIndex idx)
{
    defPlhighlight();

    core->mdat = mediaInfo->track[idx.row()];
    core->mset.reset();
    core->mset.current_id = idx.row();
    core->restarting = true;

    if (pref->play_only_this) {
      //  qDebug("Playing one track, and stop.");
        core->openFile(ui->AlbumPL->item(idx.row(), 0)->text());
    }
    else {
      //  qDebug("Playing many files:");
    /*    QString files = ui->AlbumPL->item(idx.row(),0)->text();
        for (int row=idx.row()+1; row<ui->AlbumPL->rowCount(); row++){
            files += ";" + ui->AlbumPL->item(row,0)->text();
        }
        core->openFile(files);
    */
        core->openFile(ui->AlbumPL->item(idx.row(), 0)->text());

    }

    this->setWindowTitle(parseLine(&core->mdat, pref->window_title_format));
    highlightCurrentTrack();

}


void MainWindow::playNext()
{
    qDebug() << "curId: " << core->mset.current_id << "   rowCount: " << ui->AlbumPL->rowCount();



    if ( (core->mset.current_id > -1) & (core->mset.current_id+1 < ui->AlbumPL->rowCount()) ){

        defPlhighlight();
        int row = core->mset.current_id+1;

        core->mdat = mediaInfo->track[row];
        core->mset.reset();
        core->mset.current_id = row;

        core->openFile(mediaInfo->track[row].filename);

        this->setWindowTitle(parseLine(&core->mdat, pref->window_title_format));
        highlightCurrentTrack();
    } else {
        defPlhighlight();
    }


}

void MainWindow::play(QString filename)
{
    qDebug() << "Play: " << filename;

    core->openFile(filename);
}

