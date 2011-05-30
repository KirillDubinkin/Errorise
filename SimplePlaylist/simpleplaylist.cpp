#include "simpleplaylist.h"

#include <QHeaderView>
#include <QLabel>
#include <QAction>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTimer>
#include <QTableWidgetSelectionRange>

#include <QDebug>

#include "helper.h"
#include "global.h"

using namespace Global;

const QString Cover = "c";
const QString Group = "g";

SimplePlaylist::SimplePlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    prefs = new SimplePLPrefs();
    prefsWidget = 0;  // init only on show;

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
    connect(mlib, SIGNAL(tracksSelectedBy(QStringList,QStringList)),
            this, SLOT(getNewTracks(QStringList,QStringList)));


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
}


void SimplePlaylist::loadSettings()
{
    horizontalHeader()->setVisible(prefs->show_header);
    verticalHeader()->setVisible(false);
    setAlternatingRowColors(prefs->alternate_colors);
    setStyleSheet(prefs->stylesheet);
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
    connect(act, SIGNAL(triggered()), this, SIGNAL(needPrefWindow()));
}


void SimplePlaylist::addToQueue()
{
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();

    foreach(QTableWidgetSelectionRange range, ranges)
    {
        for (int row = range.topRow(); row <= range.bottomRow(); row++)
        {
            QString s = item(row, 0)->text();
            if ((s != Group) && (s != Cover))
                player->enqueue(item(row, 0)->text().toInt());
        }
    }
}


QWidget * SimplePlaylist::getPrefsWidget()
{
    if (!prefsWidget)
    {
        prefsWidget = new SimplePlaylistPrefsWidget(prefs, this);
        connect(prefsWidget, SIGNAL(destroyed()), this, SLOT(deletePreferences()));
    }

    return prefsWidget;
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
    }
}


void SimplePlaylist::fillPlaylist()
{
    if (trackGuids.isEmpty())
        return;


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
        //! Insert row under the group, to perform image resize without resize track height
        int newRow = rowCount();
        insertRow(newRow);
        QTableWidgetItem *index = new QTableWidgetItem(Cover);
        setItem(newRow, 0, index);



        //! Set span in new row. Even if cover column not in the side of table
        if (CoverColumn == 1) {
            setSpan(newRow, 2, 1, columnCount() - 1);
            setItem(newRow, 2, newItem());
        }
        else if (CoverColumn == columnCount()) {
            setSpan(newRow, 1, 1, columnCount() - 1);
            setItem(newRow, 1, newItem());
        }
        else {
            if (CoverColumn+1 < columnCount())
                setSpan(newRow, CoverColumn+1, 1, columnCount() - CoverColumn);

            setItem(newRow, CoverColumn+1, newItem());

            if (CoverColumn > 2)
                setSpan(newRow, 1, 1, CoverColumn-1);

            setItem(newRow, 1, newItem());
        }


        setRowHeight(newRow, 0);

        //! Set span for cover cell and add him item, to set cover cell background colors
        setSpan(artRow, CoverColumn, newRow - artRow + 1, 1);
        setItem(artRow, CoverColumn, newItem());


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


QTableWidgetItem * SimplePlaylist::newItem(Qt::ItemFlags flags, const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setFlags(flags);

    return item;
}


void SimplePlaylist::addGroupItem(int row, const QString &text)
{
    this->insertRow(row);
    this->setSpan(row, 1, 1, this->columnCount()-1);

    QTableWidgetItem *group = new QTableWidgetItem(text);
    group->setTextAlignment( prefs->groups_text_aligment  | Qt::AlignVCenter);
    group->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    if (prefs->groups_text_color.isValid())
        group->setTextColor(prefs->groups_text_color);


    if (prefs->groups_back_color.isValid())
        group->setBackgroundColor(prefs->groups_back_color);
    else
        group->setBackground(this->palette().brush(QPalette::Base));


    this->setRowHeight(row, prefs->group_height);
    this->setItem(row, 1, group);

    QTableWidgetItem *index = new QTableWidgetItem(Group);
    this->setItem(row, 0, index);
}


void SimplePlaylist::addRowItem(int row, int col, const QString &text)
{
    bool ok;

    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setTextAlignment(prefs->columns_aligment.at(col));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    if (prefs->columns_text_color.at(col).isValid())
        item->setTextColor(prefs->columns_text_color.at(col));

    this->setItem(row, col+1, item);
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


    if (filename == Helper::fileName(guid))
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
            if (filename == Helper::fileName(guid))
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
                                 Helper::parseLine(this->item(currentTrackRow, 0)->text().toInt(), prefs->rows_format.at(col)));
}


void SimplePlaylist::play(int row)
{
    bool ok = 0;

    if (item(row, 0)->text() == Group)
        row++;
    int guid = this->item(row, 0)->text().toInt(&ok);

    if (ok)
        return player->play(guid);

    qWarning("SimplePlaylist::play\n\t row %d, guid %d", row, guid);
}


bool SimplePlaylist::addNextTrack()
{
    int nextTrackRow = currentTrackRow + 1;


    if ( (currentTrackRow > -1) && (nextTrackRow < rowCount()) ) //! If ok
    {
        if (item(nextTrackRow, 0)->text() == Group)
        {
            player->enqueue(item(++nextTrackRow, 0)->text().toInt());
            return 1;
        }

        if (item(nextTrackRow, 0)->text() == Cover)
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
    QSqlQuery query(mlib->db);

    value.replace("'", "''");

    if (query.exec("SELECT id FROM tracks "
                   "WHERE " + tag + " LIKE '" + value + "%'"
                   "ORDER BY filepath"))
    {
        trackGuids.clear();

        while (query.next())
            trackGuids.append(query.value(0).toInt());

        clear();
        setRowCount(0);
        currentTrackRow = -1;
        artQueue.clear();
        QTimer::singleShot(0, this, SLOT(fillPlaylist()));

    } else {
        qWarning() << query.lastError();
    }
}


void SimplePlaylist::getNewTracks(QStringList tags, QStringList values)
{
    QString temp;

    temp.append("SELECT id FROM tracks WHERE");

    while (!tags.isEmpty())
        temp.append(" (" + tags.takeFirst() + " = '" + values.takeFirst().replace("'", "''") + "') AND");

    temp.remove(temp.size() - 3, 3);
    temp.append(" ORDER BY filepath");

    QSqlQuery query(mlib->db);
    if (query.exec(temp))
    {
        trackGuids.clear();

        while (query.next())
            trackGuids.append(query.value(0).toInt());

        clear();
        setRowCount(0);
        currentTrackRow = -1;
        artQueue.clear();
        QTimer::singleShot(0, this, SLOT(fillPlaylist()));

    } else {
        qWarning() << "SimplePlaylist::getNewTracks(QStringList, QStringList)\n\t" << query.lastError();
    }

}
