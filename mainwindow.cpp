#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include "core.h"

#include <QtDebug>
#include <QLabel>

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
    ui->treeView->setModel(FSmodel);

    changeDir(pref->music_library_path);

    rx_var = QRegExp("%[a-z]*.[a-z]*%");
    /*
    rx_codec = QRegExp("codec");
    rx_bitrate = QRegExp("bitrate");
    rx_samplerate = QRegExp("samplerate");
    rx_bits = QRegExp("bits");
    rx_channels = QRegExp("channels");
    rx_playback_time = QRegExp("playback_time");
    rx_length = QRegExp("length");
    */

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));

    connect(ui->actionChoose_Directory, SIGNAL(triggered()), this, SLOT(choseAlbumDir()));
    connect(ui->actionPlay, SIGNAL(triggered()), this, SLOT(play()));
    connect(ui->AlbumPL, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playFromPL(QModelIndex)));
    connect(ui->treeView->selectionModel(),
                    SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this,
                    SLOT(directoryChanged(const QModelIndex &, const QModelIndex &)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(plFilter()));

    connect(ui->AlbumPL, SIGNAL(clicked(QModelIndex)), this, SLOT(updateStatusBar(QModelIndex)));


    createColumns();

    if (!pref->status_bar){
        ui->status->hide();
    }

}

MainWindow::~MainWindow()
{
    delete FSmodel;
    delete mediaInfo;
    delete core;
    delete ui;
}


QString MainWindow::parseLine(const int &idx, QString pattern)
{
    if (rx_var.indexIn(pattern) < 0){
        return pattern;
    } else {
        pattern.replace("%codec%", mediaInfo->track[idx].audio_codec);
        pattern.replace("%bitrate%", mediaInfo->track[idx].bitrate);
        pattern.replace("%samplerate%", mediaInfo->track[idx].samplerate);
        pattern.replace("%channels%", mediaInfo->track[idx].channels);
        pattern.replace("%length%", mediaInfo->track[idx].length);

        if (!core->playing){
            pattern.replace("%playback_time%", mediaInfo->track[idx].length);
        } else {
            // вот тут надо будет сделать _то_что_надо_.
        }
    }
    //qDebug() << pattern;
    return pattern;
}


void MainWindow::updateStatusBar(const QModelIndex &idx)
{
    ui->status->showMessage(parseLine(idx.row(), pref->status_bar_format));
}

void MainWindow::showPreferences()
{
    preferences->show();
}

// File/ChoooseDirectory -> TreeView/Update
void MainWindow::choseAlbumDir()
{
    QString fileName = QFileDialog::getExistingDirectory(this,
                              tr("Choose Your Destiny"), pref->latest_dir);
    pref->latest_dir = fileName;

    changeDir(fileName);
}

void MainWindow::changeDir(const QString fileName)
{
    if (!fileName.isEmpty()){

        ui->treeView->setRootIndex(FSmodel->index(fileName));
        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);

        pref->music_library_path = fileName;
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
    ui->AlbumPL->setRowCount(0);
    showFiles(directory, files);
}



// Создать аналогичную ф-ю, или переделать эту, для "анимированного" вывода
    // mediaInfo->parseFile
void MainWindow::showFiles(const QDir &directory, const QStringList &files, const QString &pattern)
{
    mediaInfo->parseDir(directory.absolutePath(), files);

    for (int i = 0; i < files.size(); ++i)
    {

        QTableWidgetItem *fileNameItem = new QTableWidgetItem(directory.absoluteFilePath(files[i]));

        QTableWidgetItem *titleItem = new QTableWidgetItem(mediaInfo->track[i].clip_name);
        titleItem->setFlags(Qt::ItemIsEnabled);

        QTableWidgetItem *typeItem = new QTableWidgetItem(mediaInfo->track[i].audio_format + " - "
                                                          + mediaInfo->track[i].bitrate);
        typeItem->setFlags(Qt::ItemIsEnabled);
        QTableWidgetItem *lengthItem = new QTableWidgetItem(mediaInfo->track[i].length);
        lengthItem->setFlags(Qt::ItemIsEnabled);

        int row = ui->AlbumPL->rowCount();
        ui->AlbumPL->insertRow(row);
        ui->AlbumPL->setRowHeight(row, 20);

        ui->AlbumPL->setItem(row, 0, fileNameItem);

        for (int i=0; i < pattern.length(); )
        {

            switch (pattern[i++].toAscii())
            {
            case 'N':
                ui->AlbumPL->setItem(row, i, titleItem);
                break;

            case 'T':
                ui->AlbumPL->setItem(row, i, typeItem);
                break;

            case 'L':
                ui->AlbumPL->setItem(row, i, lengthItem);
                break;

            default:
                qWarning("ui::showFiles:  Unknown character in pattern! - \"%c\"", pattern[i].toAscii());
            }


        }
        ui->AlbumPL->repaint();
    }

}


void MainWindow::setPlColumns(const QString pattern)
{
    for (int i=0; i < pattern.length(); i++)
    {
/*        int col = ui->AlbumPL->columnCount();
        ui->AlbumPL->insertColumn(col);
*/
        switch (pattern[i].toAscii())
        {
        case 'N':
            ui->AlbumPL->setColumnHidden(1, false);
            break;

        case 'T':
            ui->AlbumPL->setColumnHidden(2, false);
            break;

        case 'L':
            ui->AlbumPL->setColumnHidden(3, false);
            break;

        default:
            qWarning("ui::setPLColumns:  Unknown character in pattern! - \"%c\"", pattern[i].toAscii());
            break;
        }
    }
}

void MainWindow::createColumns(const QString &pattern)
{
        // 1st column - hidden, contains the absolute path of files
    ui->AlbumPL->setColumnCount(1);
    ui->AlbumPL->hideColumn(0);

    /*   ui->AlbumPL->insertColumn(1);
    ui->AlbumPL->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Title")));
    ui->AlbumPL->insertColumn(2);
    ui->AlbumPL->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Type")));
    ui->AlbumPL->insertColumn(3);
    ui->AlbumPL->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Length")));

    ui->AlbumPL->hideColumn(0);
    ui->AlbumPL->hideColumn(1);
    ui->AlbumPL->hideColumn(2);
    ui->AlbumPL->hideColumn(3);

    ui->AlbumPL->setColumnWidth(1, ui->AlbumPL->width() - 270);
    ui->AlbumPL->setColumnWidth(2, 150);
    ui->AlbumPL->setColumnWidth(3, 60);

   // qDebug() << ui->AlbumPL->columnCount();

    setPlColumns();
*/
    for (int i=0; i < pattern.length(); i++)
    {
       int col = ui->AlbumPL->columnCount();
       ui->AlbumPL->insertColumn(col);

       switch (pattern[i].toAscii())
       {
       case 'N':
           ui->AlbumPL->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Title")));
           ui->AlbumPL->setColumnWidth(col, ui->AlbumPL->width() - 270);
           break;

       case 'T':
           ui->AlbumPL->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Type")));
           ui->AlbumPL->setColumnWidth(col, 150);
           break;

       case 'L':
           ui->AlbumPL->setHorizontalHeaderItem(col, new QTableWidgetItem(tr("Length")));
           ui->AlbumPL->setColumnWidth(col, 60);
           break;

       default:
           qWarning("ui::createColumn:  Unknown character in pattern! - \"%c\"", pattern[i].toAscii());
       }
    }
}

void MainWindow::playFromPL(QModelIndex idx)
{
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

