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
#include <QRgb>
#include <QPushButton>
#include <QSplitter>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QPalette>

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

    this->setPalette(pref->palette);
 //   ui->AlbumPL->setPalette(pref->palette);
 //   ui->treeView->setPalette(pref->palette);

    changeAlbumDir();
    createToolBars();


    connect(ui->actionPreferences, SIGNAL(triggered()), this->preferences, SLOT(show()));
    connect(this->preferences, SIGNAL(music_folder_changed()), this, SLOT(changeAlbumDir()));
    connect(this->preferences, SIGNAL(file_filter_changed()), this, SLOT(plFilter()));
//    connect(this->preferences, SIGNAL(status_bar(bool)), this, SLOT(showStatusBar(bool)));
    connect(this->preferences, SIGNAL(hide_status_bar(bool)), ui->statusBar, SLOT(setHidden(bool)));

   // connect(this->preferences, SIGNAL(playlist_changed(QStringList,QStringList,QStringList,QStringList)),
   //         this, SLOT(changePL(QStringList,QStringList,QStringList,QStringList)));

    connect(this->preferences, SIGNAL(playlist_changed(QStringList,QStringList,QStringList,QStringList,QStringList,QStringList)),
            this, SLOT(changePL(QStringList,QStringList,QStringList,QStringList,QStringList,QStringList)));
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
    L->setSpacing(6);
    L->setMargin(1);

    L->addWidget(ui->menuMenu);
    L->addSpacing(20);
    L->addWidget(ui->mainToolBar);
    L->addWidget(progress);
    L->addWidget(vol);





  /*  QLabel *image = new QLabel(ui->splitter);
    image->setBackgroundRole(QPalette::Base);
    image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    image->setScaledContents(true);


    //ui->splitter->addWidget(image);

    image->setPixmap(QPixmap("/home/we_apon/kimi_ni_todoke_tentacles.png"));

    image->show();
*/
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
        QStringList back = pref->pl_columns_back.split("[;]");

        for (int col = 0; col < PlPattern.size(); col++)
        {
            if (pref->pl_use_html)
                this->addRowLabel(ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt(),
                                  core->mset.current_id, col, PlPattern, back);
            else
            {
                ui->AlbumPL->removeCellWidget(core->mset.current_id, col+1);
                this->addRowItem(ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt(),
                                   core->mset.current_id, col, PlPattern);
            }
        }
    }
}

void MainWindow::highlightCurrentTrack(QStringList format, QStringList back)
{
    if (core->mset.current_id == -1) // try to find
    {
        tryFindCurrentTrack();
    }

    if (core->mset.current_id > -1){

        int row = core->mset.current_id;
        int idx = ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt();

        for (int col = 0; col < format.size(); col++)
            if (!pref->pl_use_html)
            {
                bool ok;
                QTableWidgetItem *item = new QTableWidgetItem(parseLine(&mediaInfo->track[idx], format.at(col)).replace('\n', " "));
                item->setTextAlignment(QString(pref->pl_columns_aligment.at(col)).toInt());

                item->setTextColor( QColor(QString(pref->pl_color_play_text.at(col)).toInt(&ok, 16)) );
                item->setBackgroundColor( QColor(QString(pref->pl_color_play_back.at(col)).toInt(&ok, 16)) );

                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                ui->AlbumPL->setItem(row, col+1, item);
                ui->AlbumPL->setRowHeight(row, pref->pl_row_height);


            }
            else
                this->addRowLabel(idx, row, col, format, back);
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
        QStringList format = pref->pl_columns_playing_format.split("[;]");
        QStringList back = pref->pl_columns_playng_back.split("[;]");

        temp = format.at(lengthColumn() -1);

        temp.replace("%length%", MediaData::formatTime(core->mdat.duration));

        QLabel *label = new QLabel(temp);
        label->setStyleSheet("QLabel { " + back.at(lengthColumn() -1) + " }");
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
        if (ui->AlbumPL->item(i, 0)->text() != "span")
        {
            if (core->mdat.filename == mediaInfo->track[ui->AlbumPL->item(i, 0)->text().toInt()].filename){
                id = i;
            }
        }
    }
    core->mset.current_id = id;
}

void MainWindow::showPlPlaytime()
{
 //   qDebug() << "lengthColumn:" << lengthColumn();
    if (lengthColumn() > 0)
    {
   //     qDebug() << "current_id:" << core->mset.current_id;
        if (core->mset.current_id == -1) // try to find
        {
            tryFindCurrentTrack();
        }

        else if (core->mset.current_id > -1)
        {
            if (core->mdat.filename == mediaInfo->track[ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt()].filename)
            {
                QString temp;
                //PlPattern = pref->pl_columns_format.split("[;]");
                QStringList format = pref->pl_columns_playing_format.split("[;]");
                QStringList back = pref->pl_columns_playng_back.split("[;]");

                temp = format.at(lengthColumn() -1);

                temp.replace("%length%", MediaData::formatTime(core->mset.current_sec));

                QLabel *label = new QLabel(temp);
                label->setStyleSheet("QLabel { " + back.at(lengthColumn() -1) + " }");

               // qDebug() << label->text();
                ui->AlbumPL->setCellWidget(core->mset.current_id, lengthColumn(), label);
            }
            else { // Maybe, current track just go for a walk
  //              qDebug() << "go to walk id:" << core->mset.current_id;
   //             qDebug() << "mdat:" << core->mdat.filename;
    //            qDebug() << "mediaInfo:" << mediaInfo->track[ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt()].filename;
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


void MainWindow::changePL(QStringList names, QStringList format, QStringList back, QStringList playformat, QStringList playback, QStringList sizes)
{
    setPlColumns(names, sizes);

    if (pref->pl_use_groups)
        this->setPlGroupRows(format, back);
    else
        this->setPlRows(format, back);

    if (core->playing){
        highlightCurrentTrack(playformat, playback);
    }
}

void MainWindow::resetPl()
{
    setPlColumns();

    if (pref->pl_use_groups)
        this->setPlGroupRows();
    else
        this->setPlRows();

    if (core->playing){
        highlightCurrentTrack();
    }
}

void MainWindow::setPlColumns(QStringList names, QStringList sizes)
{
    if (!pref->pl_stylesheet.isEmpty())
        ui->AlbumPL->setStyleSheet("QTableWidget { " + pref->pl_stylesheet + " }");

   // setColors();

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


void MainWindow::setColors()
{
    QPalette p = ui->AlbumPL->palette();

    p.setColor(QPalette::AlternateBase, QColor(0x0C1A1A));
    p.setColor(QPalette::Highlight, QColor(0x1F2F2F));
    p.setColor(QPalette::HighlightedText, QColor(0x1F2F2F));


    ui->AlbumPL->setPalette(p);
}


void MainWindow::setPlRows(QStringList form, QStringList back)
{
    readyToPlay = false;
    int num = mediaInfo->numParsedFiles;

    ui->AlbumPL->setRowCount(num);

    int row = -1;

    for (int idx = 0; idx < num; idx++)
    {
        QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
        ui->AlbumPL->setItem(++row, 0, index);

        for (int col = 0; col < form.size(); col++)
        {
            if (pref->pl_use_html)
                this->addRowLabel(idx, row, col, form, back);
            else
                this->addRowItem(idx, row, col, form);
        }
    }
    readyToPlay = true;
}


void MainWindow::setPlGroupRows(const QStringList &form, const QStringList &back)
{
    this->readyToPlay = false;

    int num = mediaInfo->numParsedFiles, row = 0;
    QString prev, cur;

        // clear playlist
    ui->AlbumPL->setRowCount(0);

    prev = parseLine(&mediaInfo->track[0], pref->pl_groups_format);

    if (!pref->pl_use_html)
        this->addGroupItem(row, form.size(), prev);
    else
        this->addGroupLabel(row, form.size(), prev);


        // insert index of first track
    ui->AlbumPL->insertRow(++row);
    QTableWidgetItem *index = new QTableWidgetItem("0");
    ui->AlbumPL->setItem(row, 0, index);

        // fills row of first track
    for (int col = 0; col < form.size(); col++)
    {
        if (pref->pl_use_html)
            this->addRowLabel(0, row, col, form, back);
        else
            this->addRowItem(0, row, col, form);
    }



        // do the same for every next track
    for (int idx = 1; idx < num; idx++){
        cur = parseLine(&mediaInfo->track[idx], pref->pl_groups_format);

            // if group chenged - insert new group
        if (prev != cur)
        {
            if (!pref->pl_use_html)
                this->addGroupItem(++row, form.size(), cur);
            else
                this->addGroupLabel(++row, form.size(), cur);
        }

        ui->AlbumPL->insertRow(++row);
        QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
        ui->AlbumPL->setItem(row, 0, index);

        for (int col = 0; col < form.size(); col++)
        {
            if (pref->pl_use_html)
                this->addRowLabel(idx, row, col, form, back);
            else
                this->addRowItem(idx, row, col, form);
        }
        prev = cur;
    }

    this->readyToPlay = true;

   // if (!pref->pl_use_html)
   //     ui->AlbumPL->setStyleSheet("QTableWidget { " + back.at(1) + " }");

}


void MainWindow::addGroupItem(int row, int spanSize, const QString &text)
{
    bool ok;

    ui->AlbumPL->insertRow(row);
    ui->AlbumPL->setSpan(row, 1, 1, spanSize);

    QTableWidgetItem *group = new QTableWidgetItem(text);
    group->setTextAlignment( pref->pl_groups_aligment );
    group->setTextColor( QColor(pref->pl_groups_color.toInt(&ok, 16)) );
    group->setBackgroundColor( QColor(pref->pl_groups_back_color.toInt(&ok, 16)) );
    group->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    ui->AlbumPL->setItem(row, 1, group);
    ui->AlbumPL->setRowHeight(row, pref->pl_group_height);

    QTableWidgetItem *span = new QTableWidgetItem("span");
    ui->AlbumPL->setItem(row, 0, span);
}

void MainWindow::addGroupLabel(int row, int spanSize, const QString &text)
{
    ui->AlbumPL->insertRow(row);
    ui->AlbumPL->setSpan(row, 1, 1, spanSize);

    QLabel *group = new QLabel(text);
    group->setStyleSheet("QLabel { " + pref->pl_groups_back + " }");

    switch (pref->pl_groups_aligment){
    case 1: group->setAlignment(Qt::AlignLeft); break;
    case 2: group->setAlignment(Qt::AlignRight); break;
    case 4: group->setAlignment(Qt::AlignHCenter); break;
    case 8: group->setAlignment(Qt::AlignJustify); break;
    }

    ui->AlbumPL->setCellWidget(row, 1, group);
    ui->AlbumPL->setRowHeight(row, pref->pl_group_height);

    QTableWidgetItem *span = new QTableWidgetItem("span");
    ui->AlbumPL->setItem(row, 0, span);
}


void MainWindow::addRowItem(int idx, int row, int col, const QStringList &format)
{
    bool ok;
    QTableWidgetItem *item = new QTableWidgetItem(parseLine(&mediaInfo->track[idx], format.at(col)).replace('\n', " "));
    item->setTextAlignment(QString(pref->pl_columns_aligment.at(col)).toInt());
    item->setTextColor( QColor(QString(pref->pl_color_text.at(col)).toInt(&ok, 16)) );
    item->setBackgroundColor( QColor(QString(pref->pl_color_back.at(col)).toInt(&ok, 16)) );
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    ui->AlbumPL->setItem(row, col+1, item);
    ui->AlbumPL->setRowHeight(row, pref->pl_row_height);
}


void MainWindow::addRowLabel(int idx, int row, int col, const QStringList &format, const QStringList &back)
{
    QLabel *label = new QLabel(parseLine(&mediaInfo->track[idx], format.at(col)));
    label->setStyleSheet("QLabel { " + back.at(col) + " }");

    switch (QString(pref->pl_columns_aligment.at(col)).toInt()){
    case 1: label->setAlignment(Qt::AlignLeft); break;
    case 2: label->setAlignment(Qt::AlignRight); break;
    case 4: label->setAlignment(Qt::AlignHCenter); break;
    case 8: label->setAlignment(Qt::AlignJustify); break;
    }

    ui->AlbumPL->setCellWidget(row, col+1, label);
    ui->AlbumPL->setRowHeight(row, pref->pl_row_height);
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
    files.clear();

    if (pref->recursive_dirs)
        recursiveDirectory(currentPath);
    else
        files = QDir(currentPath).entryList(pref->files_filter.split(";"),
                                            QDir::Files);

    mediaInfo->parseDir(files);

    if (pref->pl_use_groups)
        this->setPlGroupRows();
    else
        this->setPlRows();

    ui->AlbumPL->setCurrentCell(0,0);

    if (core->playing){
        core->mset.current_id = -1;
        // to check, if _realy_ current track present in playlist
        highlightCurrentTrack();
    }
}


void MainWindow::recursiveDirectory(const QString &sDir)
{
    QDir dir(sDir);
    QFileInfoList list = dir.entryInfoList(pref->files_filter.split(";"), QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    for (int iList=0;iList<list.count();iList++)
    {
        QFileInfo info = list[iList];

        QString sFilePath = info.filePath();
        if (info.isDir())
        {
            // recursive
            recursiveDirectory(sFilePath);
        }
        else
        {
            files << sFilePath;
        }
    }
}


void MainWindow::playNext()
{
    if (core->mset.current_id > -1){
        if (core->mset.current_id+1 < ui->AlbumPL->rowCount())
        {
            ui->AlbumPL->setCurrentCell( core->mset.current_id+1, 0);
            play();
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
        if (ui->AlbumPL->item(ui->AlbumPL->currentRow()-1, 0)->text() == "span"){
            if (core->mset.current_id > 2){
                ui->AlbumPL->setCurrentCell( core->mset.current_id-2, 0 );
                play();
            }
        } else {
            ui->AlbumPL->setCurrentCell( core->mset.current_id-1, 0 );
            play();
        }
    }
}

void MainWindow::play()
{
    defPlhighlight();

    if (ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text() == "span"){
        ui->AlbumPL->setCurrentCell(ui->AlbumPL->currentRow()+1, 0);
    }
    int idx = ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text().toInt();

    qDebug() << "idx:" << idx;

    if (readyToPlay)
    {
        core->mdat = mediaInfo->track[idx];
        core->mset.reset();
        core->mset.current_id = ui->AlbumPL->currentRow();
        core->restarting = true;

        core->openFile(mediaInfo->track[idx].filename);

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
