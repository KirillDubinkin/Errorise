#include "simpleplaylist.h"

#include <QHeaderView>
#include <QLabel>
#include <QSettings>
#include <QTextCodec>
#include <QBrush>
#include <QTimer>
#include <QAction>
#include <QDialog>
#include <QGridLayout>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTimer>
#include <QTableWidgetSelectionRange>
#include <QHashIterator>
#include <Qt>

#include <QDebug>

#define COVER "c"
#define GROUP "g"


//! сделать авто-определение сборников
//! прятать (в идеале не заполнять вообще) колонку с артом, если группы не активны
//! придумать вкладки в плейлисте. из возможных реализаций - дерево, встроенное в таблицу,
//! либо множество экземпляров плейлиста, встроенных в дерево.
//! либо не дерево, а анимированные вкладки (нажал - группа скрылась, нажал - открылась)



SimplePlaylist::SimplePlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    prefs = new SimplePLPrefs();
    helper = new Helper();
    prefsWindow = 0;  // init only on show;

    CoverColumn     = -1;
    LengthColumn    = -1;
    currentTrackRow = -1;

    createActions();
    loadSettings();

    setColumns();
    hideColumn(0);

    setSelectionBehavior(QAbstractItemView::SelectRows);


/*//! MediaInfo
    connect(mediainfo, SIGNAL(newTracksReceived(QList<int>)),
            this, SLOT(setTracksWithGroups(QList<int>)));
*/
//!  MusicLibrary
    connect(mlib, SIGNAL(tracksSelectedBy(QString,QString)),
            this, SLOT(getNewTracks(QString,QString)));


//!  Player (PhononFace)
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(play(int)));

    connect(player, SIGNAL(trackChanged(QString,int)),
            this, SLOT(highlightCurrentTrack(QString,int)));

    connect(player, SIGNAL(aboutToFinish()), this, SLOT(addNextTrack()));
    connect(player, SIGNAL(finished()), this, SLOT(finished()));
    connect(player, SIGNAL(needNext()), this, SLOT(playNext()));
//! //////////////////////////////////////////////////////////////////////////


}

SimplePlaylist::~SimplePlaylist()
{
    delete prefs;
    delete helper;
}


void SimplePlaylist::loadSettings()
{
    this->horizontalHeader()->setVisible(prefs->show_header);
    this->verticalHeader()->setVisible(false);
    this->setAlternatingRowColors(prefs->alternate_colors);
    this->setStyleSheet(prefs->stylesheet);
}

void SimplePlaylist::createActions()
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *act;

    act = new QAction(tr("Add to playback queue"), this);
    addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(addToQueue()));

    act = new QAction(this);
    act->setSeparator(true);
    addAction(act);

    act = new QAction(tr("Preferences..."), this);
    addAction(act);
    connect(act, SIGNAL(triggered()), this, SLOT(showPreferences()));
}


void SimplePlaylist::addToQueue()
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();

    foreach(QTableWidgetSelectionRange range, ranges)
    {
        for (int row = range.topRow(); row <= range.bottomRow(); row++)
        {
            QString s = item(row, 0)->text();
            if ((s != GROUP) && (s != COVER))
                player->enqueue(item(row, 0)->text().toInt());
        }
    }
}


void SimplePlaylist::showPreferences()
{
    if (!prefsWindow)
    {
        prefsWindow = new SimplePlaylistPrefsWindow(prefs, this);

        connect(prefsWindow, SIGNAL(showHeaderChanged(bool)),
                this->horizontalHeader(), SLOT(setVisible(bool)));
        connect(prefsWindow, SIGNAL(useAlternateColorsChanged(bool)),
                this, SLOT(setAlternatingRowColors(bool)));
        connect(prefsWindow, SIGNAL(styleEdited(QString)),
                this, SLOT(setStyleSheet(QString)));
    }

    QDialog *dialog = new QDialog(this);
    QGridLayout *l  = new QGridLayout(this);
    l->addWidget(prefsWindow);
    dialog->setLayout(l);
    dialog->show();
}


int SimplePlaylist::coverColumn()
{
    return CoverColumn;
}

int SimplePlaylist::lengthColumn()
{
    return LengthColumn;
}

void SimplePlaylist::setColumns()
{
    // first column contains index of track. it's hidden column;
    this->setColumnCount(1);
    int col=1;

    this->CoverColumn = -1;

    for (int i = 0; i < prefs->columns_names.size(); i++)
    {
        if (QString(prefs->rows_format.at(i)).contains("%art%"))
            this->CoverColumn = col;

        this->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem(prefs->columns_names.at(i));

        this->setHorizontalHeaderItem(col, item);
        this->setColumnWidth(col++, prefs->columns_sizes.at(i));

     //   qDebug() << "PL:" << prefs->columns_names.at(i) << "size =" << prefs->columns_sizes.at(i);
    }

   // qDebug() << "PL:ColumnCount" << this->columnCount();
   // qDebug() << "PL:CoverColumn" << this->CoverColumn;
}


void SimplePlaylist::setTracks()
{
    //qDebug() << "PL:GUID.size" << GUID.size();
    //qDebug() << "ID's :" << GUID;

    this->setRowCount(trackGuids.size());
   // int row = -1;

    int colCount = this->columnCount() - 1;

    if (!prefs->labels){
            //! In this function row == idx, so
        for (int idx = 0; idx < trackGuids.size(); idx++){
            this->setRowHeight(idx, prefs->row_height);

            QTableWidgetItem *index = new QTableWidgetItem(QString().number(trackGuids.at(idx)));
            this->setItem(idx, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowItem(idx, col, helper->parseLine(trackGuids.at(idx), prefs->rows_format.at(col)));
        }
    } else {
        for (int idx = 0; idx < trackGuids.size(); idx++){
            this->setRowHeight(idx, prefs->row_height);

            QTableWidgetItem *index = new QTableWidgetItem(QString().number(trackGuids.at(idx)));
            this->setItem(idx, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowLabel(idx, col, helper->parseLine(trackGuids.at(idx), prefs->rows_format.at(col)));
        }
    }
}


void SimplePlaylist::fillPlaylist()
{
    if (trackGuids.isEmpty())
        return;

    int groupRow = 0;

    QList<int> group;
    group.append(trackGuids.first());

    QString dir  = Helper::valueOfTrack("filepath", trackGuids.first());
    QString prev = helper->parseLine(trackGuids.takeFirst(), prefs->groups_format);
    QString current;
    bool VA = false;

    int index=1;
    while (!trackGuids.isEmpty())
    {
        int guid = trackGuids.first();
        current = helper->parseLine(guid, prefs->groups_format);

        if (prev != current)
        {
            if ( (index - groupRow > prefs->group_delay) | (dir != Helper::valueOfTrack("filepath", guid)) )
                break;
            else
                VA = true;

            groupRow = index;
        }

        prev = current;
        group.append(trackGuids.takeFirst());
        index++;
    }



    int colCount = this->columnCount() - 1;
    int row = this->rowCount();

    if (prefs->group_header)
    {
        if (VA)
            this->addGroupItem(row++, Helper::vaGroup(prefs->groups_format));
            //this->addGroupItem(row++, prev);
        else
            this->addGroupItem(row++, prev);
    }

    int artRow = row;


    for (int idx = 0; idx < group.size(); idx++)
    {
        this->insertRow(row);
        this->setRowHeight(row, prefs->row_height);

        QTableWidgetItem *guid = new QTableWidgetItem(QString::number(group.at(idx)));
        this->setItem(row, 0, guid);

        for (int col = 0; col < colCount; col++)
            if (this->CoverColumn-1 != col)
                this->addRowItem(row, col, helper->parseLine(group.at(idx), prefs->rows_format.at(col)));
        row++;
    }




    if (CoverColumn > -1)
    {
        if (row - artRow > 1)
            setSpan(artRow, CoverColumn, row - artRow, 1);
        setItem(artRow, CoverColumn, newItem(palette().brush(QPalette::Base), Qt::NoItemFlags)); // заменить на крутотушку

        QString cover = Helper::valueOfTrack("art", group.first());
        if (!cover.isEmpty())
            insertCover(artRow, CoverColumn, cover);
    }


    QTimer::singleShot(0, this, SLOT(highlightCurrentTrack()));
    QTimer::singleShot(0, this, SLOT(fillPlaylist()));

}



void SimplePlaylist::insertCover(int row, int col, const QString &cover)
{
    QPixmap fullPic(cover);
    QPixmap pic = fullPic.scaledToWidth(prefs->columns_sizes.at(CoverColumn - 1),( Qt::TransformationMode) prefs->smooth_art_scale);

    int curGroupSize = prefs->row_height * (rowCount() - row);

    if (curGroupSize < pic.height())
    {
        const QBrush &brush = this->palette().brush(QPalette::Base);

        //! Insert row under the group, to perform image resize without resize track height
        int newRow = rowCount();
        this->insertRow(newRow);
        QTableWidgetItem *index = new QTableWidgetItem(COVER);
        this->setItem(newRow, 0, index);



        //! Set span in new row. Even if cover column not in the side of table
        if (this->CoverColumn == 1) {
            this->setSpan(newRow, 2, 1, this->columnCount() - 1);
            this->setItem(newRow, 2, this->newItem(brush, Qt::NoItemFlags));
        }
        else if (this->CoverColumn == this->columnCount()) {
            this->setSpan(newRow, 1, 1, this->columnCount() - 1);
            this->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
        }
        else {
            if (this->CoverColumn+1 < this->columnCount())
                this->setSpan(newRow, this->CoverColumn+1, 1, this->columnCount() - this->CoverColumn);

            this->setItem(newRow, this->CoverColumn+1, this->newItem(brush, Qt::NoItemFlags));

            if (this->CoverColumn > 2)
                this->setSpan(newRow, 1, 1, this->CoverColumn-1);

            this->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
        }


        this->setRowHeight(newRow,  pic.height() - curGroupSize);

        //! Set span for cover cell and add him item, to set cover cell background colors
        this->setSpan(row, col, newRow - row + 1, 1);
        this->setItem(row, col, newItem(brush, Qt::NoItemFlags));
        //this->setCellWidget(row, col, art);

    }

    QLabel *art = new QLabel;
    art->setMaximumHeight(pic.height());

    setCellWidget(row, col, art);
    art->setPixmap(pic);
}


void SimplePlaylist::setTracksWithGroups()
{
    //! Exit, if files not found, or somethink, to avoid empty group creation
    if (trackGuids.isEmpty())
        return;

    this->currentTrackRow = -1;

    //! Clear. We don't know how many rows will be
    this->setRowCount(0);
    int groupRow = 0, row = 0, colCount = this->columnCount() - 1;    

    //! Insert first group
    QString current;
    QString prev = helper->parseLine(trackGuids.at(0), prefs->groups_format);
    if (!prefs->group_labels)
        this->addGroupItem(row, prev);
    else
        this->addGroupLabel(row, prev);

//    if (!prefs->labels) {
        for (int idx = 0; idx < trackGuids.size(); idx++)
        {
            current = helper->parseLine(trackGuids.at(idx), prefs->groups_format);

            if (prev != current)
            {
                if (row - groupRow > 1)
                { //! Insert cover to current group, and begin a new group
                    if (this->CoverColumn > -1)
                        row = this->addCover(groupRow+1, row - groupRow, helper->filePath(trackGuids.at(idx-1)));

                   if (!prefs->group_labels)
                        this->addGroupItem(++row, current);
                    else
                        this->addGroupLabel(++row, current);
                }

                groupRow = row;

            }


            this->insertRow(++row);
            this->setRowHeight(row, prefs->row_height);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(trackGuids.at(idx)));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowItem(row, col, helper->parseLine(trackGuids.at(idx), prefs->rows_format.at(col)));

            prev = current;
        }
//    }

/*
    //! Do the same, except this->addRowLabel;
    //! The whole algorithm is written twice, because otherwise this test will be called (columnCount * trackGuids.size()) times
    else {
        for (int idx = 0; idx < trackGuids.size(); idx++)
        {
            current = helper->parseLine(trackGuids.at(idx), prefs->groups_format);

            if (prev != current)
            { //! Insert cover to current group, and begin a new group
                if (this->CoverColumn > -1)
                    row = this->addCover(groupRow+1, row - groupRow, helper->filePath(trackGuids.at(idx-1)));

                if (!prefs->group_labels)
                    this->addGroupItem(++row, current);
                else
                    this->addGroupLabel(++row, current);

                groupRow = row;
            }

            this->insertRow(++row);
            this->setRowHeight(row, prefs->row_height);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(trackGuids.at(idx)));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowItem(row, col, helper->parseLine(trackGuids.at(idx), prefs->rows_format.at(col)));

            prev = current;
        }
    }
*/

    if (this->CoverColumn > -1)
        row = this->addCover(groupRow+1, row - groupRow, helper->filePath(trackGuids.last()));


    QTimer::singleShot(0, this, SLOT(highlightCurrentTrack()));
}


int SimplePlaylist::addCover(int row, int spanRow, const QString &searchPath)
{
    const QBrush &brush = this->palette().brush(QPalette::Base);

    if (!pref->art_search_patterns.isEmpty())
    {

        QDir path(searchPath);
        QStringList files = path.entryList(pref->art_search_patterns, QDir::Files);

        if (!files.isEmpty())
        {

            QPixmap dPic(path.absoluteFilePath(files.at(0)));
            QPixmap pic = dPic.scaledToWidth(prefs->columns_sizes.at(this->CoverColumn-1),
                                             Qt::SmoothTransformation);

           // float factor = (float) prefs->columns_sizes.at(this->CoverColumn-1) / pic.width();
            int curGroupSize = prefs->row_height * (spanRow);


            QLabel *art = new QLabel;
            //art->setScaledContents(true);
            art->setPixmap(pic);

            if (curGroupSize < pic.height())
            {
                //! Insert row under the group, to perform image resize without resize track height
                int newRow = row + spanRow;
                this->insertRow(newRow);
                QTableWidgetItem *index = new QTableWidgetItem(COVER);
                this->setItem(newRow, 0, index);



                //! Set span in new row. Even if cover column not in the side of table
                if (this->CoverColumn == 1) {
                    this->setSpan(newRow, 2, 1, this->columnCount() - 1);
                    this->setItem(newRow, 2, this->newItem(brush, Qt::NoItemFlags));
                }
                else if (this->CoverColumn == this->columnCount()) {
                    this->setSpan(newRow, 1, 1, this->columnCount() - 1);
                    this->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
                }
                else {
                    if (this->CoverColumn+1 < this->columnCount())
                        this->setSpan(newRow, this->CoverColumn+1, 1, this->columnCount() - this->CoverColumn);

                    this->setItem(newRow, this->CoverColumn+1, this->newItem(brush, Qt::NoItemFlags));

                    if (this->CoverColumn > 2)
                        this->setSpan(newRow, 1, 1, this->CoverColumn-1);

                    this->setItem(newRow, 1, this->newItem(brush, Qt::NoItemFlags));
                }


                this->setRowHeight(newRow,  pic.height() - curGroupSize);

                //! Set span for cover cell and add him item, to set cover cell background colors
                this->setSpan(row, this->CoverColumn, spanRow+1, 1);
                this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));
                this->setCellWidget(row, this->CoverColumn, art);

                return newRow;
            }

            this->setSpan(row, this->CoverColumn, spanRow, 1);
            this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));

            art->setMaximumHeight(pic.height());
            this->setCellWidget(row, this->CoverColumn, art);

            return row + spanRow - 1;
        }
    }

    //! Set span for cover cell, if images not found, or pattern is empty.
    if (spanRow > 1)
    {
        this->setSpan(row, this->CoverColumn, spanRow, 1);
        this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));

        return row + spanRow - 1;
    }

    return row;
}


QTableWidgetItem * SimplePlaylist::newItem(const QBrush &background, Qt::ItemFlags flags, const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setFlags(flags);
    item->setBackground(background);

    return item;
}


void SimplePlaylist::addGroupItem(int row, const QString &text)
{
    this->insertRow(row);
    this->setSpan(row, 1, 1, this->columnCount()-1);

    QTableWidgetItem *group = new QTableWidgetItem(text);
    group->setTextAlignment( prefs->groups_text_aligment  | Qt::AlignVCenter);
    group->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    bool ok;
    if (!prefs->groups_text_color.isEmpty())
        group->setTextColor( QColor(prefs->groups_text_color.toInt(&ok, 16)) );

    if (prefs->groups_back_color.isEmpty())
        group->setBackground(this->palette().brush(QPalette::Base));
    else
        group->setBackgroundColor( QColor(prefs->groups_back_color.toInt(&ok, 16)) );

    this->setRowHeight(row, prefs->group_height);
    this->setItem(row, 1, group);

    QTableWidgetItem *index = new QTableWidgetItem(GROUP);
    this->setItem(row, 0, index);
}

void SimplePlaylist::addGroupLabel(int row, const QString &text)
{
    this->insertRow(row);
    this->setSpan(row, 1, 1, this->columnCount()-1);

    QLabel *group = new QLabel(text);

    switch (prefs->groups_text_aligment){
    case 1: group->setAlignment(Qt::AlignLeft | Qt::AlignVCenter); break;
    case 2: group->setAlignment(Qt::AlignRight | Qt::AlignVCenter); break;
    case 4: group->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); break;
    case 8: group->setAlignment(Qt::AlignJustify | Qt::AlignVCenter); break;
    }

    if (!prefs->groups_stylesheet.isEmpty())
        group->setStyleSheet("QLabel { " + prefs->groups_stylesheet + " }");

    this->setRowHeight(row, prefs->group_height);
    this->setCellWidget(row, 1, group);

    QTableWidgetItem *index = new QTableWidgetItem(GROUP);
    this->setItem(row, 0, index);
}


void SimplePlaylist::addRowItem(int row, int col, const QString &text)
{
    bool ok;

    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(prefs->columns_aligment.at(col));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (prefs->color_column_text.at(col) != "")
        item->setTextColor( QColor(QString(prefs->color_column_text.at(col)).toInt(&ok, 16)) );

    if (!prefs->alternate_colors)
    {
        if (prefs->color_column_back.at(col) != "")
            item->setBackgroundColor( QColor(QString(prefs->color_column_back.at(col)).toInt(&ok, 16)) );
        else
            item->setBackground(this->palette().brush(QPalette::Base));
    }

    this->setItem(row, col+1, item);
}

void SimplePlaylist::addRowLabel(int row, int col, const QString &text)
{
    QLabel *label = new QLabel(text);

    switch (prefs->columns_aligment.at(col)) {
    case 1: label->setAlignment(Qt::AlignLeft); break;
    case 2: label->setAlignment(Qt::AlignRight); break;
    case 4: label->setAlignment(Qt::AlignHCenter); break;
    case 8: label->setAlignment(Qt::AlignJustify); break;
    }

    if (prefs->columns_stylesheet.at(col) != "")
        label->setStyleSheet("QLabel { " + prefs->columns_stylesheet.at(col) + " }");

    this->setCellWidget(row, col+1, label);
}


void SimplePlaylist::highlightCurrentTrack()
{
    int guid = player->currentGuid();

    if (guid == -1)
        return;

    this->highlightCurrentTrack(player->currentTrack(), guid);
}

void SimplePlaylist::highlightCurrentTrack(QString filename, int guid)
{
    if (this->currentTrackRow > -1)
        this->defPlhighlight();   // clear previos highlighting


    if (filename == helper->fileName(guid))
        findCurrentTrack(guid);
    else
        findCurrentTrack(filename);

    if (currentTrackRow > -1)
        for (int j = 1; j < this->columnCount(); j++)
        {
            if (j != this->CoverColumn)
            {
                item(currentTrackRow, j)->setBackground(palette().brush(QPalette::Active, QPalette::Highlight));
                item(currentTrackRow, j)->setTextColor(palette().brush(QPalette::Active, QPalette::HighlightedText).color());
            }
        }
/*
    else
        qDebug("PL-HIGHLIGHT: Current track not found");
*/
}


bool SimplePlaylist::findCurrentTrack(int guid)
{
    for (int i = 0; i < rowCount(); i++)
    {
        if (item(i, 0)->text() == QString::number(guid))
        {
            this->currentTrackRow = i;

            return 1;
        }
    }
    return 0;
}


bool SimplePlaylist::findCurrentTrack(QString filename)
{
    bool ok;
    int guid;

    for (int i = 0; i < rowCount(); i++)
    {
        guid = item(i, 0)->text().toInt(&ok);
        if (ok)
        {
            if (filename == helper->fileName(guid))
            {
                this->currentTrackRow = i;
                return 1;
            }
        }
    }

    return 0;
}

void SimplePlaylist::defPlhighlight()
{
    if (currentTrackRow > -1)
        for (int col = 1; col < columnCount() - 1; col++)
            if (col != CoverColumn - 1)
                this->addRowItem(currentTrackRow, col,
                                 helper->parseLine(this->item(currentTrackRow, 0)->text().toInt(), prefs->rows_format.at(col)));
}


void SimplePlaylist::play(int row)
{
    bool ok = 0;

    if (item(row, 0)->text() == GROUP)
        row++;
    int guid = this->item(row, 0)->text().toInt(&ok);

    if (ok)
        return player->play(guid);

    qWarning("Playlist: row %d, guid %d", row, guid);
}


bool SimplePlaylist::addNextTrack()
{
    int nextTrackRow = currentTrackRow + 1;


    if ( (currentTrackRow > -1) && (nextTrackRow < rowCount()) ) //! If ok
    {
        if (item(nextTrackRow, 0)->text() == GROUP)
        {
            player->enqueue(item(++nextTrackRow, 0)->text().toInt());
            return 1;
        }

        if (item(nextTrackRow, 0)->text() == COVER)
        {
            if (++nextTrackRow < rowCount())       //! After cover may be only group
                {                                      //! so, play group+1 row
                    player->enqueue(item(++nextTrackRow, 0)->text().toInt());
                    return 1;
                }
            return 0;
        }

        player->enqueue(item(nextTrackRow, 0)->text().toInt());
        return 1;
    }

    return 0;
}


void SimplePlaylist::finished()
{
    this->defPlhighlight();
    this->currentTrackRow = -1;
}


void SimplePlaylist::playNext()
{
    if (addNextTrack())
    {
        player->stop();
        player->play();
    }
}


void SimplePlaylist::getNewTracks(QString tag, QString value)
{
    //QList<QMap<QMap<QString

    QSqlQuery query(mlib->db);

    value.replace("'", "''");

    if (query.exec("SELECT id FROM tracks "
                   "WHERE " + tag + " LIKE '" + value + "%'"
                   "ORDER BY filepath, filename"))
    {
        trackGuids.clear();

        while (query.next())
        {
            trackGuids.append(query.value(0).toInt());
            //qDebug() << query.value(0).toInt();
        }

        //QTimer::singleShot(0, this, SLOT(setTracksWithGroups()));

        this->clear();
        this->setRowCount(0);
        this->currentTrackRow = -1;
        QTimer::singleShot(0, this, SLOT(fillPlaylist()));

        //this->setTracksWithGroups(trackGuids);

    } else {
        qWarning() << query.lastError();
    }
}
