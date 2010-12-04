#include "simpleplaylist.h"
#include "paths.h"
#include <QHeaderView>
#include <QLabel>
#include <QSettings>
#include <QTextCodec>

#include <QDebug>

#define COVER "c"
#define GROUP "g"
#define PLUG_NAME "SimplePlayList"

Prefs::Prefs()
{
    filename = Paths::iniPath() + "/" + PLUG_NAME + ".ini";
    reset();
    load();
}

Prefs::reset()
{
    qDebug("SimplePlaylist->Prefs::reset");

    //! Groups
    groups_text_color = "ffffff";
    groups_text_aligment = Qt::AlignLeft;
    groups_back_color = "13363b";
    group_height = 20;
}

Prefs::save()
{
    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    set.setValue("groups_text_color", groups_text_color);
    set.setValue("groups_back_color", groups_back_color);
    set.setValue("groups_text_aligment", groups_text_aligment);
    set.setValue("group_height", group_height);
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    set.setValue("columns_names", columns_names);
    set.setValue("columns_sizes", columns_sizes);

    set.endGroup();


    //! Other
    set.beginGroup("Other");
    set.setValue("art_search_pattern", art_search_pattern);
    set.setValue("row_height", row_height);
    set.endGroup();


    set.sync();
}

Prefs::load()
{
    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    groups_text_color = set.value("groups_text_color", groups_text_color).toString();
    groups_back_color = set.value("groups_back_color", groups_back_color).toString();
    groups_text_aligment = set.value("groups_text_aligment", groups_text_aligment).toInt();
    group_height = set.value("group_height", group_height).toInt();
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    columns_names = set.value("columns_names", columns_names).toStringList();
    columns_sizes = set.value("columns_sizes", columns_sizes).toList();
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    art_search_pattern = set.value("art_search_pattern", art_search_pattern).toString();
    row_height = set.value("row_height", row_height);
}





SimplePlaylist::SimplePlaylist(bool showColumnsNames, QWidget *parent) :
    QTableWidget(parent)
{
    prefs = new Prefs();

    this->horizontalHeader()->setVisible(showColumnsNames);
    this->hideColumn(0);
    CoverColumn  = -1;
    LengthColumn = -1;
}

int SimplePlaylist::coverColumn()
{
    return CoverColumn;
}

int SimplePlaylist::lengthColumn()
{
    return LengthColumn;
}

void SimplePlaylist::setColumns(const QStringList &names, const QStringList &sizes, const QStringList &rowsFormat)
{
    // first column contains index of track. it's hidden column;
    this->setColumnCount(1);
    int col=1;

    this->CoverColumn = -1;

    for (int i = 0; i < names.size(); i++)
    {
        if (QString(rowsFormat.at(i)).contains("%art%"))
            this->CoverColumn = col;

        this->insertColumn(col);
        QTableWidgetItem *item = new QTableWidgetItem(names.at(i));

        this->setHorizontalHeaderItem(col, item);
        this->setColumnWidth(col++, QString(sizes.at(i)).toInt());
    }
}


void SimplePlaylist::setTracks(const QStringList &text, const int &num, bool useLabels)
{
    this->setRowCount(num);
    int row = -1;

    //! Meybe It's need to replase with this->ColumnCount - 1;
    int colCount = text[num].size();

    if (!useLabels){
        for (int idx = 0; idx < num; idx++){
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
            this->setItem(++row, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowItem(idx, row, col);
        }
    } else {
        for (int idx = 0; idx < num; idx++){
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
            this->setItem(++row, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowLabel(idx, row, col);
        }
    }
}


void SimplePlaylist::setTracksWithGroups(const QStringList &text, const int &num,
                                        const QStringList &group, const QList<int> &groupIndex,
                                        const QStringList &groupPath,
                                        bool useLabels, bool useGroupLabels)
{
    // Clear
    this->setRowCount(0);
    int groupID = 0, row = 0, colCount = text[num].size();

    if (!useLabels){
        for (int idx = 0; idx < num; idx++){
            if (idx == groupIndex.at(groupID)){
                if (this->CoverColumn > -1)
                   row = this->addCover(groupIndex.at(groupID)+1, row - groupIndex.at(groupID), groupPath.at(groupID));
            //! We search for groups and covers in Main Program, and send it here.

                if (!useGroupLabels)
                    this->addGroupItem(++row, group.at(++groupID));
                else
                    this->addGroupLabel(++row, group.at(++groupID));
            }

            this->insertRow(++row);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowItem(idx, row, col);
        }
    }


    else {
        for (int idx = 0; idx < num; idx++){
            if (idx == groupIndex.at(groupID)){
                if (this->CoverColumn > -1)
                   row = this->addCover(groupIndex.at(groupID)+1, row - groupIndex.at(groupID), groupPath.at(groupID));

                if (!useGroupLabels)
                    this->addGroupItem(++row, group.at(++groupID));
                else
                    this->addGroupLabel(++row, group.at(++groupID));
            }

            this->insertRow(++row);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(idx));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowLabel(idx, row, col);
        }
    }
}


int SimplePlaylist::addCover(int row, int spanRow, const QString &searchPath)
{

    if (!prefs->art_search_pattern.isEmpty())
    {


#ifdef Q_OS_WIN
        qt_ntfs_permission_lookup++;

#endif

        QDir path(searchPath);
        QStringList files = path.entryList(prefs->art_search_pattern, QDir::Files);

        if (!files.isEmpty())
        {

            QPixmap pic(path.absoluteFilePath(files.at(0)));

#ifdef Q_OS_WIN
            qt_ntfs_permission_lookup--;
#endif
            float factor = (float) prefs->columns_sizes.at(this->CoverColumn-1) / pic.width();
            int curGroupSize = prefs->row_height * (spanRow+2);

            QLabel *art = new QLabel;
            art->setScaledContents(true);
            art->setPixmap(pic);

            if (curGroupSize < (pic.height() * factor))
            {
                //! Insert row under the group, to perform image resize without resize track height
                int newRow = row + spanRow;
                this->insertRow(newRow);
                QTableWidgetItem *index = new QTableWidgetItem(COVER);
                this->setItem(newRow, 0, index);

                const QBrush &brush = this->palette().brush(QPalette::Base);

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


                this->setRowHeight(newRow,  pic.height() * factor - curGroupSize);

                //! Set span for cover cell and add him item, to set cover cell background colors
                this->setSpan(row, this->CoverColumn, spanRow+1, 1);
                this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));
                this->setCellWidget(row, this->CoverColumn, art);

                return newRow;
            }

            this->setSpan(row, this->CoverColumn, spanRow, 1);
            this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));

            art->setMaximumHeight(pic.height() * factor);
            this->setCellWidget(row, this->CoverColumn, art);

            return row + spanRow - 1;
        }
    }

    //! Set span for cover cell, if images not found, or pattern is empty.
    this->setSpan(row, this->CoverColumn, spanRow, 1);
    this->setItem(row, this->CoverColumn, this->newItem(brush, Qt::NoItemFlags));

    return row + spanRow - 1;
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
