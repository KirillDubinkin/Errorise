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
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPixmap>
#include <QPalette>
#include <QDesktopWidget>
#include <QDockWidget>

#ifdef Q_OS_WIN
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

using namespace Global;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(pref->x, pref->y, pref->res_main_width, pref->res_main_height);
    this->setStyleSheet(pref->main_stylesheet);

    ui->AlbumPL->setStyleSheet(pref->pl_stylesheet);
    ui->AlbumPL->setAlternatingRowColors(pref->pl_alternate_colors);
    ui->AlbumPL->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *act;
    ui->AlbumPL->addAction(act = new QAction("Add to Playback Queue", ui->AlbumPL));
    connect(act, SIGNAL(triggered()), this, SLOT(addToQueue()));


    /*QDockWidget *dock = new QDockWidget(tr("Albumlist"));
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui->treeView->setMaximumWidth(2435354);
    ui->treeView->setParent(dock);
    dock->setWidget(ui->treeView);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Playlist"));
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    ui->AlbumPL->setParent(dock);
    dock->setWidget(ui->AlbumPL);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
*/




    defWindowTitle();

    core = new Core(this);
    mediaInfo = new MediaInfo(this);
    preferences = new PreferencesWindow(this);

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
    connect(this->preferences, SIGNAL(hide_status_bar(bool)), ui->statusBar, SLOT(setHidden(bool)));


    connect(this->preferences, SIGNAL(playlist_changed()), this, SLOT(changePL()));

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
    connect(this->core, SIGNAL(playnext()), this, SLOT(defPlhighlight()));
    connect(this->core, SIGNAL(playnext()), this, SLOT(playNext()));


   // connect(this->progress, SIGNAL(sliderMoved(int)), this, SLOT(setTime(int)));
  //  connect(this->progress, SIGNAL(valueChanged(int)), this, SLOT(setTime(int)));
    connect(this->progress, SIGNAL(sliderReleased()), this, SLOT(setTime()));

 //   connect(this->vol, SIGNAL(sliderMoved(int)), this, SLOT(setVol(int)));
    connect(this->vol, SIGNAL(valueChanged(int)), this, SLOT(setVol(int)));



        //Stylesheets
    connect(preferences, SIGNAL(mainStyleChanged(QString)), this, SLOT(setStyleSheet(QString)));
    connect(preferences, SIGNAL(plStyleChanged(QString)), ui->AlbumPL, SLOT(setStyleSheet(QString)));


    setPlColumns();
    ui->AlbumPL->hideColumn(0);
    ui->AlbumPL->horizontalHeader()->setVisible(false);

    ui->statusBar->setHidden(!pref->status_bar);


    lengthColumn();


    //insertToolBars();

    ui->treeView->setGeometry(0,0,pref->res_tree_width, 0);

}

MainWindow::~MainWindow()
{
    pref->res_tree_width = ui->treeView->width();
#ifdef Q_OS_WIN
    pref->x = this->x()+2;
    pref->y = this->y()+20;
#else
    pref->x = this->x();
    pref->y = this->y();
#endif
    pref->res_main_width = this->width();
    pref->res_main_height = this->height();
    pref->res_pref_width = preferences->width();
    pref->res_pref_height = preferences->height();
    delete FSmodel;
    delete mediaInfo;
    delete core;
    delete ui;
}

void MainWindow::addToQueue()
{
    queue->addQueue(mediaInfo->track[ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text().toInt()]);
}

void MainWindow::createToolBars()
{
    progress = new MySlider();

    vol = new MySlider();
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


void MainWindow::setTime()
{
    if (core->playing)
        core->goToSec(progress->value());
    else
        this->progress->setValue(0);
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
    if (core->mset.current_id > -1)
        for (int col = 0; col < pref->pl_columns_format.size(); col++)
            if (this->coverColumn-1 != col){
                if (pref->pl_use_html)
                    this->addRowLabel(ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt(),
                                  core->mset.current_id, col);
                else {
                    ui->AlbumPL->removeCellWidget(core->mset.current_id, col+1);
                    this->addRowItem(ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt(),
                                   core->mset.current_id, col);
                }
            }
}

void MainWindow::highlightCurrentTrack()
{
    if (core->mset.current_id == -1) // try to find
        tryFindCurrentTrack();

    if (core->mset.current_id > -1)
    {
        int row = core->mset.current_id;
        int idx = ui->AlbumPL->item(core->mset.current_id, 0)->text().toInt();

        for (int col = 0; col < pref->pl_columns_playing_format.size(); col++)
            if (this->coverColumn-1 != col){
                if (!pref->pl_use_html)
                {
                    bool ok;
                    QTableWidgetItem *item = new QTableWidgetItem(parseLine(&mediaInfo->track[idx], pref->pl_columns_playing_format.at(col)).replace('\n', " "));
                    item->setTextAlignment(QString(pref->pl_columns_aligment.at(col)).toInt());


                    if (pref->pl_color_play_text.at(col) != "")
                        item->setTextColor( QColor(QString(pref->pl_color_play_text.at(col)).toInt(&ok, 16)) );
                    else
                        item->setTextColor(ui->AlbumPL->palette().color(QPalette::Active, QPalette::HighlightedText));

                    if (pref->pl_color_play_back.at(col) != "")
                        item->setBackgroundColor( QColor(QString(pref->pl_color_play_back.at(col)).toInt(&ok, 16)) );
                    else
                        item->setBackground(ui->AlbumPL->palette().brush(QPalette::Active, QPalette::Highlight));


                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                    ui->AlbumPL->setItem(row, col+1, item);
                    ui->AlbumPL->setRowHeight(row, pref->pl_row_height);
                }
                else
                    this->addRowLabel(idx, row, col);
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

    if (!progress->isSliderDown()){
        //this->dont_change_time = true;
        progress->blockSignals(true);
        progress->setValue(core->mset.current_sec);
        progress->blockSignals(false);
        //this->dont_change_time = false;
    }
}


void MainWindow::showDefTimePl()
{
    if (lengthColumn() > 0)
    {
        QString temp;
        temp = pref->pl_columns_playing_format.at(lengthColumn() -1);

        temp.replace("%length%", MediaData::formatTime(core->mdat.duration));

        QLabel *label = new QLabel(temp);
        label->setStyleSheet("QLabel { " + pref->pl_columns_playng_back.at(lengthColumn() -1) + " }");
        ui->AlbumPL->setCellWidget(core->mset.current_id, lengthColumn(), label);
    }
}


void MainWindow::defWindowTitle()
{
    this->setWindowTitle(myplayerName() + " v." + myplayerVersion());
}


void MainWindow::tryFindCurrentTrack()
{
    int id = -2; // We try just once


    for (int i = 0; i < ui->AlbumPL->rowCount(); i++)
    {
        if (ui->AlbumPL->item(i, 0)->text() != "span")
        {
            if (core->mdat.filename == mediaInfo->track[ui->AlbumPL->item(i, 0)->text().toInt()].filename){
                core->mset.current_id = i;
                return;
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

                temp = pref->pl_columns_playing_format.at(lengthColumn() -1);

                temp.replace("%length%", MediaData::formatTime(core->mset.current_sec));

                QLabel *label = new QLabel(temp);
                label->setStyleSheet("QLabel { " + pref->pl_columns_playng_back.at(lengthColumn() -1) + " }");

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
        for (int i = 0; i < pref->pl_columns_format.size(); i++){
            s = pref->pl_columns_format.at(i);
            if (s.contains("%length%")){
                timeColumn = i+1;
                return timeColumn;
            }
        }
        timeColumn = -1;
    }
    return timeColumn;
}


void MainWindow::changePL()
{
    setPlColumns();

    if (!files.isEmpty()){
        if (pref->pl_use_groups)
            this->setPlGroupRows();
        else
            this->setPlRows();
    }

    if (core->playing){
        highlightCurrentTrack();
    }
}


void MainWindow::setPlColumns()
{
    // first - absolete file path - hidden
    ui->AlbumPL->setColumnCount(1);
    int col=1;

    this->coverColumn = -1;

    for (int i = 0; i < pref->pl_columns_names.size(); i++)
    {
        if (QString(pref->pl_columns_format.at(i)).contains("%art%"))
            this->coverColumn = col;

        ui->AlbumPL->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem(pref->pl_columns_names.at(i));

        ui->AlbumPL->setHorizontalHeaderItem(col, item);
        ui->AlbumPL->setColumnWidth(col++, QString(pref->pl_columns_sizes.at(i)).toInt());

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


void MainWindow::setPlRows()
{
    readyToPlay = false;
    int num = mediaInfo->numParsedFiles;

    ui->AlbumPL->setRowCount(num);

    int row = -1;

    for (int idx = 0; idx < num; idx++)
    {
        QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
        ui->AlbumPL->setItem(++row, 0, index);

        for (int col = 0; col < pref->pl_columns_format.size(); col++)
        {
            if (pref->pl_use_html)
                this->addRowLabel(idx, row, col);
            else
                this->addRowItem(idx, row, col);
        }
    }
    readyToPlay = true;
}


void MainWindow::setPlGroupRows()
{
    this->readyToPlay = false;

    int num = mediaInfo->numParsedFiles, row = 0, prevGroupRow=0;
    QString prev, cur;

        // clear playlist
    ui->AlbumPL->setRowCount(0);

    prev = parseLine(&mediaInfo->track[0], pref->pl_groups_format);

    if (!pref->pl_groups_labels)
        this->addGroupItem(row, prev);
    else
        this->addGroupLabel(row, prev);


        // insert index of first track
    ui->AlbumPL->insertRow(++row);
    QTableWidgetItem *index = new QTableWidgetItem("0");
    ui->AlbumPL->setItem(row, 0, index);

        // fills row of first track
    for (int col = 0; col < pref->pl_columns_format.size(); col++)
    {
        if (this->coverColumn-1 != col){
            if (pref->pl_use_html)
                this->addRowLabel(0, row, col);
            else
                this->addRowItem(0, row, col);
        }
    }



        // do the same for every next track
    for (int idx = 1; idx < num; idx++){
        cur = parseLine(&mediaInfo->track[idx], pref->pl_groups_format);

            // if group chenged - insert new group
        if (prev != cur)
        {
            if (this->coverColumn > -1){

                row = this->addCover(prevGroupRow+1, row-prevGroupRow, QFileInfo(mediaInfo->track[idx-1].filename).absolutePath());

                //row++;
            }


            if (!pref->pl_groups_labels)
                this->addGroupItem(++row, cur);
            else
                this->addGroupLabel(++row, cur);

            prevGroupRow = row;
        }

        ui->AlbumPL->insertRow(++row);
        QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
        ui->AlbumPL->setItem(row, 0, index);

        for (int col = 0; col < pref->pl_columns_format.size(); col++)
        {
            if (this->coverColumn-1 != col){
                if (pref->pl_use_html)
                    this->addRowLabel(idx, row, col);
                else
                    this->addRowItem(idx, row, col);
            }
        }
        prev = cur;
    }

    if (this->coverColumn > -1)
        row = this->addCover(prevGroupRow+1, row-prevGroupRow, QFileInfo(mediaInfo->track[num-1].filename).absolutePath());

    this->readyToPlay = true;

   // if (!pref->pl_use_html)
   //     ui->AlbumPL->setStyleSheet("QTableWidget { " + back.at(1) + " }");

}


int MainWindow::addCover(int row, int spanRow, const QString &searchPath)
{
    const QBrush &brush = this->palette().brush(QPalette::Base);

    if (!pref->pl_art_search_pattern.isEmpty())
    {


#ifdef Q_OS_WIN
        qt_ntfs_permission_lookup++;

#endif

        QDir path(searchPath);
        QStringList files = path.entryList(pref->pl_art_search_pattern, QDir::Files);

        if (files.isEmpty()){
            if (path.exists(searchPath + "/art"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Art"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Scans"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/scans"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Covers"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/covers"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else
                path.cdUp();

            files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
        }


        if (files.isEmpty()){
            if (path.exists(searchPath + "/art"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Art"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Scans"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/scans"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/Covers"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
            else if (path.exists(searchPath + "/covers"))
                files = path.entryList(pref->pl_art_search_pattern, QDir::Files);
        }



        if (!files.isEmpty())
        {

            QPixmap pic(path.absoluteFilePath(files.at(0)));

#ifdef Q_OS_WIN
            qt_ntfs_permission_lookup--;
#endif
            float factor = (float) QString(pref->pl_columns_sizes.at(this->coverColumn-1)).toInt() / pic.width();
            int curGroupSize = pref->pl_row_height * (spanRow+2);



            QLabel *art = new QLabel;
            art->setScaledContents(true);
            art->setPixmap(pic);

            if (curGroupSize < (pic.height() * factor))
            {
                //! Insert row under the group, to perform image resize without resize track height
                int newRow = row + spanRow;
                ui->AlbumPL->insertRow(newRow);
                QTableWidgetItem *index = new QTableWidgetItem("art");
                ui->AlbumPL->setItem(newRow, 0, index);



                //! Set span in new row. Even if cover column not in the side of table
                if (this->coverColumn == 1) {
                    ui->AlbumPL->setSpan(newRow, 2, 1, ui->AlbumPL->columnCount() - 1);
                    ui->AlbumPL->setItem(newRow, 2, this->newItem(brush, Qt::NoItemFlags));
                }
                else if (this->coverColumn == ui->AlbumPL->columnCount()) {
                    ui->AlbumPL->setSpan(newRow, 1, 1, ui->AlbumPL->columnCount() - 1);
                    ui->AlbumPL->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
                }
                else {
                    if (this->coverColumn+1 < ui->AlbumPL->columnCount())
                        ui->AlbumPL->setSpan(newRow, this->coverColumn+1, 1, ui->AlbumPL->columnCount() - this->coverColumn);

                    ui->AlbumPL->setItem(newRow, this->coverColumn+1, this->newItem(brush, Qt::NoItemFlags));

                    if (this->coverColumn > 2)
                        ui->AlbumPL->setSpan(newRow, 1, 1, this->coverColumn-1);

                    ui->AlbumPL->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
                }


                ui->AlbumPL->setRowHeight(newRow,  pic.height() * factor - curGroupSize);

                //! Set span for cover cell and add him item, to set cover cell background colors
                ui->AlbumPL->setSpan(row, this->coverColumn, spanRow+1, 1);
                ui->AlbumPL->setItem(row, this->coverColumn, this->newItem(brush, Qt::NoItemFlags));
                ui->AlbumPL->setCellWidget(row, this->coverColumn, art);

                return newRow;
            }

            ui->AlbumPL->setSpan(row, this->coverColumn, spanRow, 1);
            ui->AlbumPL->setItem(row, this->coverColumn, this->newItem(brush, Qt::NoItemFlags));

            art->setMaximumHeight(pic.height() * factor);
            ui->AlbumPL->setCellWidget(row, this->coverColumn, art);

            return row + spanRow - 1;
        }
    }

    //! Set span for cover cell, if images not found, or pattern is empty.
    ui->AlbumPL->setSpan(row, this->coverColumn, spanRow, 1);
    ui->AlbumPL->setItem(row, this->coverColumn, this->newItem(brush, Qt::NoItemFlags));

    return row + spanRow - 1;
}


void MainWindow::addCover(int row, int spanRow, const QDir &path)
{
    int newRow = row + spanRow;
    ui->AlbumPL->insertRow(newRow);
    QTableWidgetItem *index = new QTableWidgetItem("art");
    ui->AlbumPL->setItem(newRow, 0, index);
    ui->AlbumPL->setRowHeight(newRow, 0);

    const QBrush &brush = ui->AlbumPL->palette().brush(QPalette::Base);

    if (this->coverColumn == 1)
    {
        ui->AlbumPL->setSpan(newRow, 2, 1, pref->pl_columns_names.size() - 1);
        ui->AlbumPL->setItem(newRow, 2, this->newItem(brush, Qt::NoItemFlags));
    }
    else if (this->coverColumn == pref->pl_columns_names.size())
    {
        ui->AlbumPL->setSpan(newRow, 1, 1, pref->pl_columns_names.size() - 1);
        ui->AlbumPL->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
    }
    else
    {
        if (this->coverColumn+1 < pref->pl_columns_names.size())
            ui->AlbumPL->setSpan(newRow, this->coverColumn+1, 1, pref->pl_columns_names.size() - this->coverColumn);

        ui->AlbumPL->setItem(newRow, this->coverColumn+1, this->newItem(brush, Qt::NoItemFlags));

        if (this->coverColumn > 2)
            ui->AlbumPL->setSpan(newRow, 1, 1, this->coverColumn-1);

        ui->AlbumPL->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));

    }

    ui->AlbumPL->setSpan(row, this->coverColumn, spanRow+1, 1);
    ui->AlbumPL->setItem(row, this->coverColumn, this->newItem(brush, Qt::NoItemFlags));

    if (!pref->pl_art_search_pattern.isEmpty())
    {


#ifdef Q_OS_WIN
        qt_ntfs_permission_lookup++;

#endif

        QStringList files = path.entryList(pref->pl_art_search_pattern, QDir::Files);

        if (!files.isEmpty())
        {

            QPixmap pic(path.absoluteFilePath(files.at(0)));

#ifdef Q_OS_WIN
            qt_ntfs_permission_lookup--;
#endif
            float factor = (float) QString(pref->pl_columns_sizes.at(this->coverColumn-1)).toInt() / pic.width();
            int curGroupSize = pref->pl_row_height * (spanRow+2);

            QLabel *art = new QLabel;
            art->setScaledContents(true);
            art->setPixmap(pic);

            if (curGroupSize < (pic.height() * factor))
                ui->AlbumPL->setRowHeight(newRow,  pic.height() * factor - curGroupSize);
            else
                art->setMaximumHeight(pic.height() * factor);

            ui->AlbumPL->setCellWidget(row, this->coverColumn, art);
        }
    }
}


QTableWidgetItem * MainWindow::newItem(const QBrush &background, Qt::ItemFlags flags, const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setFlags(flags);
    item->setBackground(background);

    return item;
}


void MainWindow::addGroupItem(int row, const QString &text)
{
    bool ok;

    ui->AlbumPL->insertRow(row);
    ui->AlbumPL->setSpan(row, 1, 1, pref->pl_columns_format.size());

    QTableWidgetItem *group = new QTableWidgetItem(text);
    group->setTextAlignment( pref->pl_groups_aligment  | Qt::AlignVCenter);
    group->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (!pref->pl_groups_text_color.isEmpty())
        group->setTextColor( QColor(pref->pl_groups_text_color.toInt(&ok, 16)) );

    if (!pref->pl_groups_back_color.isEmpty())
        group->setBackgroundColor( QColor(pref->pl_groups_back_color.toInt(&ok, 16)) );
    else
        group->setBackground(ui->AlbumPL->palette().brush(QPalette::Base));



    ui->AlbumPL->setRowHeight(row, pref->pl_group_height);
    ui->AlbumPL->setItem(row, 1, group);

    QTableWidgetItem *span = new QTableWidgetItem("span");
    ui->AlbumPL->setItem(row, 0, span);
}

void MainWindow::addGroupLabel(int row, const QString &text)
{
    ui->AlbumPL->insertRow(row);
    ui->AlbumPL->setSpan(row, 1, 1, pref->pl_columns_format.size());

    QLabel *group = new QLabel(text);
    group->setStyleSheet("QLabel { " + pref->pl_groups_stylesheet + " }");

    switch (pref->pl_groups_aligment){
    case 1: group->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); break;
    case 2: group->setAlignment(Qt::AlignRight | Qt::AlignVCenter); break;
    case 4: group->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); break;
    case 8: group->setAlignment(Qt::AlignJustify | Qt::AlignVCenter); break;
    }

    ui->AlbumPL->setRowHeight(row, pref->pl_group_height);
    ui->AlbumPL->setCellWidget(row, 1, group);

    QTableWidgetItem *span = new QTableWidgetItem("span");
    ui->AlbumPL->setItem(row, 0, span);
}


void MainWindow::addRowItem(int idx, int row, int col)
{
    bool ok;

    QTableWidgetItem *item = new QTableWidgetItem(parseLine(&mediaInfo->track[idx], pref->pl_columns_format.at(col)).replace('\n', " "));
    item->setTextAlignment(QString(pref->pl_columns_aligment.at(col)).toInt());
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (pref->pl_color_text.at(col) != "")
        item->setTextColor( QColor(QString(pref->pl_color_text.at(col)).toInt(&ok, 16)) );

    if (!pref->pl_alternate_colors)
    {
        if (pref->pl_color_back.at(col) != "")
            item->setBackgroundColor( QColor(QString(pref->pl_color_back.at(col)).toInt(&ok, 16)) );
        else
            item->setBackground(ui->AlbumPL->palette().brush(QPalette::Base));
    }


    ui->AlbumPL->setRowHeight(row, pref->pl_row_height);
    ui->AlbumPL->setItem(row, col+1, item);
}


void MainWindow::addRowLabel(int idx, int row, int col)
{
    QLabel *label = new QLabel(parseLine(&mediaInfo->track[idx], pref->pl_columns_format.at(col)));
    label->setStyleSheet("QLabel { " + pref->pl_columns_back.at(col) + " }");

    switch (QString(pref->pl_columns_aligment.at(col)).toInt()){
    case 1: label->setAlignment(Qt::AlignLeft); break;
    case 2: label->setAlignment(Qt::AlignRight); break;
    case 4: label->setAlignment(Qt::AlignHCenter); break;
    case 8: label->setAlignment(Qt::AlignJustify); break;
    }

    ui->AlbumPL->setRowHeight(row, pref->pl_row_height);
    ui->AlbumPL->setCellWidget(row, col+1, label);
}


QString MainWindow::parseLine(MediaData *data, QString pattern)
{
    pattern.replace("%title%", data->clip_name);
    pattern.replace("%artist%", data->clip_artist);
    pattern.replace("%album_artist%", data->album_artist);
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

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

    if (pref->recursive_dirs)
        recursiveDirectory(currentPath);
    else
        files = QDir(currentPath).entryList(pref->files_filter.split(";"),
                                            QDir::Files);

    if (files.isEmpty())
        return;

    mediaInfo->parseDir(files);

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

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
    qDebug() << queue->size();
    if (queue->size() > 0){
        this->playQueue(queue->currentIdx());
        queue->setCurrent(queue->currentIdx()+1);
        return;
    }

    if (core->mset.current_id > -1){
        if (core->mset.current_id+2 < ui->AlbumPL->rowCount())
        {
            ui->AlbumPL->setCurrentCell( core->mset.current_id+1, 0);
            qDebug() << ui->AlbumPL->currentRow();
            play();
        }
    } else {
        if (ui->AlbumPL->currentRow() <= ui->AlbumPL->rowCount())
        {
            play();
        }
    }
}


void MainWindow::playQueue(int idx)
{
    progress->blockSignals(true);
    progress->setValue(0);
    progress->blockSignals(false);

    if (queue->size() > idx)
    {
        defPlhighlight();

        core->mdat = queue->queue(idx);
        core->mset.reset();
        core->mset.current_id = -1;
        core->restarting = true;

        core->openFile(core->mdat.filename);

        this->setWindowTitle(parseLine(&core->mdat, pref->window_title_format));
        highlightCurrentTrack();

        /////////////////////
//        progress->setMinimum(0);

        progress->setMaximum(core->mdat.duration);

        if ((pref->pl_auto_scroll) && (ui->AlbumPL->columnCount() > 0) && (core->mset.current_id > -1))
        {
            if (this->coverColumn == 1)
                ui->AlbumPL->scrollToItem(ui->AlbumPL->item(ui->AlbumPL->currentRow(), ui->AlbumPL->columnCount()-1));
            else
                ui->AlbumPL->scrollToItem(ui->AlbumPL->item(ui->AlbumPL->currentRow(), 1));
        }
    } else {
        queue->clear();
        this->playNext();
    }

}


void MainWindow::playPrev()
{
    if ( (core->playing) & (core->mset.current_id > 0) ){
        if (ui->AlbumPL->item(ui->AlbumPL->currentRow()-1, 0)->text() == "span"){
            if (core->mset.current_id > 2)
            {
                ui->AlbumPL->setCurrentCell( core->mset.current_id-2, 0 );

                if (ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text() == "art")
                    ui->AlbumPL->setCurrentCell(ui->AlbumPL->currentRow()-1, 0);

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
    qDebug() << "play()";

    progress->blockSignals(true);
    progress->setValue(0);
    progress->blockSignals(false);

    if (ui->AlbumPL->rowCount() <= 0)
        return;

    if (ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text() == "art")
            ui->AlbumPL->setCurrentCell(ui->AlbumPL->currentRow()+2, 0);

    else if (ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text() == "span")
            ui->AlbumPL->setCurrentCell(ui->AlbumPL->currentRow()+1, 0);

    if ((ui->AlbumPL->currentRow() < 0) | (ui->AlbumPL->currentRow() > ui->AlbumPL->rowCount()))
        return;


    int idx = ui->AlbumPL->item(ui->AlbumPL->currentRow(), 0)->text().toInt();

    qDebug() << "idx:" << idx;

    if (readyToPlay)
    {
        defPlhighlight();

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

        if ((pref->pl_auto_scroll) && (ui->AlbumPL->columnCount() > 0))
        {
            if (this->coverColumn == 1)
                ui->AlbumPL->scrollToItem(ui->AlbumPL->item(ui->AlbumPL->currentRow(), ui->AlbumPL->columnCount()-1));
            else
                ui->AlbumPL->scrollToItem(ui->AlbumPL->item(ui->AlbumPL->currentRow(), 1));
        }
    }
}


void MainWindow::playPause()
{
    qDebug() << "core->playing" << core->playing;
    if (core->playing)
        core->pause();
    else
        play();
}

void MainWindow::stop()
{
    this->progress->setValue(0);
    core->restarting=true;
    defPlhighlight();
    core->stop();
}
