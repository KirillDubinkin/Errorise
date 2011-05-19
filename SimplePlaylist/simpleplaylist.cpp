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


void SimplePlaylist::fillPlaylist()
{
    if (trackGuids.isEmpty())
    {
//        QTimer::singleShot(0, this, SLOT(insertLastCovers()));
        return;
    }

    int groupRow = 0;

    QList<int> group;
    group.append(trackGuids.first());

    QString dir  = Helper::valueOfTrack("filedir", trackGuids.first());
    QString prev = Helper::parseLine(trackGuids.takeFirst(), prefs->groups_format);
    QString current;
    bool VA = false;

    int index=1;
    while (!trackGuids.isEmpty())
    {
        int guid = trackGuids.first();
        current = Helper::parseLine(guid, prefs->groups_format);

        if (prefs->group_byDirs)
        {
            if (dir != Helper::valueOfTrack("filedir", guid))
                break;

            if (prev != current)
                VA = true;

        } else if (prev != current) {
            if ( (index - groupRow > prefs->group_delay) | (dir != Helper::valueOfTrack("filedir", guid)) )
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
            addGroupItem(row++, Helper::vaGroup(prefs->groups_format));
        else
            addGroupItem(row++, prev);
    }

    int artRow = row;


    for (int idx = 0; idx < group.size(); idx++)
    {
        insertRow(row);
        setRowHeight(row, prefs->row_height);

        QTableWidgetItem *guid = new QTableWidgetItem(QString::number(group.at(idx)));
        setItem(row, 0, guid);

        for (int col = 0; col < colCount; col++)
            if (CoverColumn-1 != col)
                addRowItem(row, col, Helper::parseLine(group.at(idx), prefs->rows_format.at(col)));
        row++;
    }




    if (CoverColumn > -1)
    {
        const QBrush &brush = palette().brush(QPalette::Base);

        //! Insert row under the group, to perform image resize without resize track height
        int newRow = rowCount();
        insertRow(newRow);
        QTableWidgetItem *index = new QTableWidgetItem(COVER);
        setItem(newRow, 0, index);



        //! Set span in new row. Even if cover column not in the side of table
        if (CoverColumn == 1) {
            setSpan(newRow, 2, 1, columnCount() - 1);
            setItem(newRow, 2, newItem(brush, Qt::NoItemFlags));
        }
        else if (CoverColumn == columnCount()) {
            setSpan(newRow, 1, 1, columnCount() - 1);
            setItem(newRow, 1, newItem(brush, Qt::NoItemFlags));
        }
        else {
            if (CoverColumn+1 < columnCount())
                setSpan(newRow, CoverColumn+1, 1, columnCount() - CoverColumn);

            setItem(newRow, CoverColumn+1, newItem(brush, Qt::NoItemFlags));

            if (CoverColumn > 2)
                setSpan(newRow, 1, 1, CoverColumn-1);

            setItem(newRow, 1, newItem(brush, Qt::NoItemFlags));
        }


        setRowHeight(newRow, 0);

        //! Set span for cover cell and add him item, to set cover cell background colors
        setSpan(artRow, CoverColumn, newRow - artRow + 1, 1);
        setItem(artRow, CoverColumn, newItem(brush, Qt::NoItemFlags));


        QString cover = Helper::valueOfTrack("playlistart", group.first());
        if (!cover.isEmpty())
            artQueue.append(CoversQueue(artRow, newRow, cover));
        else
        {
            cover = Helper::valueOfTrack("art", group.first());
            if (!cover.isEmpty())
            {
                cover = doPlaylistArt(cover, dir);
                artQueue.append(CoversQueue(artRow, newRow, cover));
            }
        }

        QTimer::singleShot(0, this, SLOT(insertCover()));

    }


    QTimer::singleShot(0, this, SLOT(highlightCurrentTrack()));
    QTimer::singleShot(0, this, SLOT(fillPlaylist()));

}


QString SimplePlaylist::doPlaylistArt(QString filename, QString dir)
{
    QPixmap pic = QPixmap(filename).scaledToWidth(prefs->columns_sizes.at(CoverColumn - 1), Qt::SmoothTransformation);

    QString newFilename = dir.append("/" + pref->pl_art_filename);
    if (pic.save(newFilename))
    {
        QSqlQuery query(mlib->db);

        if (!query.exec("UPDATE tracks SET playlistart = '" + QString(newFilename).replace("'", "''") + "' "
                        "WHERE art LIKE '" + filename.replace("'", "''") + "'"))
            qWarning() << "SimplePlaylist::doPlaylistArt\n\t" << query.lastError().text();

        return newFilename;
    }

    qWarning() << "SimplePlaylist::doPlaylistArt  New picture not saved!";
    return filename;
}


void SimplePlaylist::insertCover()
{
    if (artQueue.isEmpty())
        return;

    CoversQueue art = artQueue.takeFirst();

    //QPixmap pic = QPixmap(art.filename).scaledToWidth(prefs->columns_sizes.at(CoverColumn - 1),( Qt::TransformationMode) prefs->smooth_art_scale);
    QPixmap pic(art.filename);

    int curGroupSize = prefs->row_height * (art.spanRow - art.artRow);
    int picHeight    = pic.height();

    if (curGroupSize < picHeight)
        setRowHeight(art.spanRow, picHeight - curGroupSize);

    QLabel *cover = new QLabel;
    cover->setMaximumHeight(picHeight);

    setCellWidget(art.artRow, CoverColumn, cover);
    cover->setPixmap(pic);
}


void SimplePlaylist::insertLastCovers()
{
    if (artQueue.isEmpty())
        return;

    CoversQueue art = artQueue.takeFirst();

    //QPixmap pic = QPixmap(art.filename).scaledToWidth(prefs->columns_sizes.at(CoverColumn - 1),( Qt::TransformationMode) prefs->smooth_art_scale);
    QPixmap pic(art.filename);

    int curGroupSize = prefs->row_height * (art.spanRow - art.artRow);
    int picHeight    = pic.height();

    if (curGroupSize < picHeight)
        setRowHeight(art.spanRow, picHeight - curGroupSize);

    QLabel *cover = new QLabel;
    cover->setMaximumHeight(picHeight);

    setCellWidget(art.artRow, CoverColumn, cover);
    cover->setPixmap(pic);

    QTimer::singleShot(0, this, SLOT(insertLastCovers()));
}


QTableWidgetItem * SimplePlaylist::newItem(const QBrush &background, Qt::ItemFlags flags, const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setFlags(flags);
//    item->setBackground(background);

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


/*
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
*/


void SimplePlaylist::addRowItem(int row, int col, const QString &text)
{
    bool ok;

    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(prefs->columns_aligment.at(col));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (prefs->color_column_text.at(col) != "")
        item->setTextColor( QColor(QString(prefs->color_column_text.at(col)).toInt(&ok, 16)) );

/*    if (!prefs->alternate_colors)
    {
        if (prefs->color_column_back.at(col) != "")
            item->setBackgroundColor( QColor(QString(prefs->color_column_back.at(col)).toInt(&ok, 16)) );
        else
            item->setBackground(this->palette().brush(QPalette::Base));
    }
*/
    this->setItem(row, col+1, item);
}


/*
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
*/


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
                   "ORDER BY filepath"))
    {
        trackGuids.clear();

        while (query.next())
        {
            trackGuids.append(query.value(0).toInt());
            //qDebug() << query.value(0).toInt();
        }

        //QTimer::singleShot(0, this, SLOT(setTracksWithGroups()));

        clear();
        setRowCount(0);
        currentTrackRow = -1;
        artQueue.clear();
        QTimer::singleShot(0, this, SLOT(fillPlaylist()));


        //this->setTracksWithGroups(trackGuids);

    } else {
        qWarning() << query.lastError();
    }
}
