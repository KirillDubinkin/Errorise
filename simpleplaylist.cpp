#include "simpleplaylist.h"
#include "paths.h"

#include <QHeaderView>
#include <QLabel>
#include <QSettings>
#include <QTextCodec>
#include <QBrush>

#include <QDebug>

#define COVER "c"
#define GROUP "g"
#define PLUG_NAME "SimplePlayList"


//! сделать авто-определение сборников
//! прятать (в идеале не заполнять вообще) колонку с артом, если группы не активны
//! придумать вкладки в плейлисте. из возможных реализаций - дерево, встроенное в таблицу,
//! либо множество экземпляров плейлиста, встроенных в дерево.
//! либо не дерево, а анимированные вкладки (нажал - группа скрылась, нажал - открылась)

Prefs::Prefs()
{
    filename = Paths::iniPath() + "/" + PLUG_NAME + ".ini";
    qDebug() << "SimplePlaylist->Prefs::path" << filename;
    reset();
    load();
}

Prefs::~Prefs()
{
    save();
}

void Prefs::reset()
{
    qDebug("SimplePlaylist->Prefs::reset()");

    //! Groups
    groups_format = "%album_artist% - [%date%] %album%";
    groups_text_color = "ffffff";
    groups_text_aligment = Qt::AlignLeft;
    groups_back_color = "13363b";
    groups_stylesheet = "background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.681, fx:0.5, fy:0.5, stop:0 rgba(0, 35, 51, 255), stop:1 rgba(0, 74, 92, 255));\ncolor: rgb(255, 255, 255);";
    group_height = 20;
    group_labels = true;

    //! Columns
    columns_names << "Cover" << "#" << "Length" << "Track Name" << "Bitrate" << "Format";
    columns_sizes << 200 << 23 << 75 << 200 << 75 << 50;
    columns_aligment << 4 << 4 << 4 << 4 << 4 << 4;
    columns_stylesheet << "" << "" << "" << "" << "" << "";

    //! Rows
    rows_format << "%art%" << "%tracknumber%" << "%length%" << "%title%" << "%bitrate%" << "%codec%";
    rows_stylesheet << "" << "" << "" << "" << "" << "";
    rows_playback_format << "%art%" << "%tracknumber%" << "%length%" << "%title%" << "%bitrate%" << "%codec%";
    rows_playback_stylesheet << "" << "" << "" << "" << "" << "";
    labels = false;

    //! Colors
    color_column_text << "" << "527482" << "43606b" << "" << "43606b" << "527482";
    color_column_back << "" << "" << "" << "" << "" << "";

    //! Other
    art_search_pattern << "*cover*.jpg" << "*folder*.jpg" << "*front*.jpg";
    row_height = 14;
    alternate_colors = true;
    show_header = false;


}

void Prefs::save()
{
    qDebug("SimplePlaylist->Prefs::save()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    set.setValue("groups_format", groups_format);
    set.setValue("groups_text_color", groups_text_color);
    set.setValue("groups_back_color", groups_back_color);
    set.setValue("groups_text_aligment", groups_text_aligment);
    set.setValue("groups_stylesheet", groups_stylesheet);
    set.setValue("group_height", group_height);
    set.setValue("group_labels", group_labels);
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    set.setValue("columns_names", columns_names);

    s_col_size.clear();
    for (int i = 0; i < columns_sizes.size(); i++)
        s_col_size << QString().number(columns_sizes.at(i));
    set.setValue("columns_sizes", s_col_size);

    s_col_align.clear();
    for (int i = 0; i < columns_aligment.size(); i++)
        s_col_align << QString().number(columns_aligment.at(i));
    set.setValue("columns_aligment", s_col_align);

    set.setValue("columns_stylesheet", columns_stylesheet);
    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    set.setValue("rows_format", rows_format);
    set.setValue("rows_stylesheet", rows_stylesheet);
    set.setValue("rows_playback_format", rows_playback_format);
    set.setValue("rows_playback_stylesheet", rows_playback_stylesheet);
    set.setValue("labels", labels);
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    set.setValue("color_column_text", color_column_text);
    set.setValue("color_column_back", color_column_back);
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    set.setValue("art_search_pattern", art_search_pattern);
    set.setValue("row_height", row_height);
    set.setValue("alternate_colors", alternate_colors);
    set.setValue("show_header", show_header);

    set.endGroup();


    set.sync();
}

void Prefs::load()
{
    qDebug("SimplePlaylist->Prefs::load()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Groups
    set.beginGroup("Groups");
    groups_format = set.value("groups_format", groups_format).toString();
    groups_text_color = set.value("groups_text_color", groups_text_color).toString();
    groups_back_color = set.value("groups_back_color", groups_back_color).toString();
    groups_text_aligment = set.value("groups_text_aligment", groups_text_aligment).toInt();
    groups_stylesheet = set.value("groups_stylesheet", groups_stylesheet).toString();
    group_height = set.value("group_height", group_height).toInt();
    group_labels = set.value("group_labels", group_labels).toBool();
    set.endGroup();


    //! Columns
    set.beginGroup("Columns");
    columns_names = set.value("columns_names", columns_names).toStringList();

    s_col_size = set.value("columns_sizes", s_col_size).toStringList();
    if (!s_col_size.isEmpty()) {
        columns_sizes.clear();
        for (int i = 0; i < s_col_size.size(); i++)
            columns_sizes << QString(s_col_size.at(i)).toInt();
    }

    s_col_align = set.value("columns_aligment", s_col_align).toStringList();
    if (!s_col_align.isEmpty()) {
        columns_aligment.clear();
        for (int i = 0; i < s_col_align.size(); i++)
            columns_aligment << QString(s_col_align.at(i)).toInt();
    }

    columns_stylesheet = set.value("columns_stylesheet", columns_stylesheet).toStringList();
    set.endGroup();


    //! Rows
    set.beginGroup("Rows");
    rows_format = set.value("rows_format", rows_format).toStringList();
    rows_stylesheet = set.value("rows_stylesheet", rows_stylesheet).toStringList();
    rows_playback_format = set.value("rows_playback_format", rows_playback_format).toStringList();
    rows_playback_stylesheet = set.value("rows_playback_stylesheet", rows_playback_stylesheet).toStringList();
    labels = set.value("labels", labels).toBool();
    set.endGroup();


    //! Colors
    set.beginGroup("Colors");
    color_column_text = set.value("color_column_text", color_column_text).toStringList();
    color_column_back = set.value("color_column_back", color_column_back).toStringList();
    set.endGroup();


    //! Other
    set.beginGroup("Other");
    art_search_pattern = set.value("art_search_pattern", art_search_pattern).toStringList();
    row_height = set.value("row_height", row_height).toInt();
    alternate_colors = set.value("alternate_colors", alternate_colors).toBool();
    show_header = set.value("show_header", show_header).toBool();
    set.endGroup();
}





SimplePlaylist::SimplePlaylist(QWidget *parent) :
    QTableWidget(parent)
{
    prefs = new Prefs();
    helper = new Helper();

    this->horizontalHeader()->setVisible(prefs->show_header);
    this->hideColumn(0);
    CoverColumn  = -1;
    LengthColumn = -1;
}

SimplePlaylist::~SimplePlaylist()
{
    delete prefs;

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
        this->setColumnWidth(col++, QString(prefs->columns_sizes.at(i)).toInt());
    }
}


void SimplePlaylist::setTracks(const QList<int> &GUID)
{
    this->setRowCount(GUID.size());
   // int row = -1;

    int colCount = this->columnCount() - 1;

    if (!prefs->labels){
            //! In this function row == idx, so
        for (int idx = 0; idx < GUID.size(); idx++){
            this->setRowHeight(idx, prefs->row_height);

            QTableWidgetItem *index = new QTableWidgetItem(QString().number(GUID.at(idx)));
            this->setItem(idx, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowItem(idx, col, helper->parseLine(GUID.at(idx), prefs->rows_format.at(col)));
        }
    } else {
        for (int idx = 0; idx < GUID.size(); idx++){
            this->setRowHeight(idx, prefs->row_height);

            QTableWidgetItem *index = new QTableWidgetItem(QString().number(GUID.at(idx)));
            this->setItem(idx, 0, index);

            for (int col = 0; col < colCount; col++)
                this->addRowLabel(idx, col, helper->parseLine(GUID.at(idx), prefs->rows_format.at(col)));
        }
    }
}


void SimplePlaylist::setTracksWithGroups(const QList<int> &GUID)
{
    //! Exit, if files not found, or somethink, to avoid empty group creation
    if (GUID.isEmpty())
        return;

    //! Clear. We don't know how many rows will be
    this->setRowCount(0);
    int groupRow = 0, row = 0, colCount = this->columnCount() - 1;

    //! Insert first group
    QString prev = helper->parseLine(GUID.at(0), prefs->groups_format), current;
    if (!prefs->group_labels)
        this->addGroupItem(row++, prev);
    else
        this->addGroupLabel(row++, prev);

    if (!prefs->labels) {
        for (int idx = 0; idx < GUID.size(); idx++)
        {
            current = helper->parseLine(GUID.at(idx), prefs->groups_format);

            if (prev != current)
            { //! Insert cover to current group, and begin a new group
                if (this->CoverColumn > -1)
                    row = this->addCover(groupRow+1, row - groupRow, helper->filePath(GUID.at(idx-1)));

                if (!prefs->group_labels)
                    this->addGroupItem(++row, current);
                else
                    this->addGroupLabel(++row, current);
            }

            this->insertRow(++row);
            this->setRowHeight(row, prefs->row_height);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(GUID.at(idx)));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowItem(row, col, helper->parseLine(GUID.at(idx), prefs->rows_format.at(col)));

            prev = current;
        }
    }

    //! Do the same, except this->addRowLabel;
    //! The whole algorithm is written twice, because otherwise this test will be called (columnCount * GUID.size()) times
    else {
        for (int idx = 0; idx < GUID.size(); idx++)
        {
            current = helper->parseLine(GUID.at(idx), prefs->groups_format);

            if (prev != current)
            { //! Insert cover to current group, and begin a new group
                if (this->CoverColumn > -1)
                    row = this->addCover(groupRow+1, row - groupRow, helper->filePath(GUID.at(idx-1)));

                if (!prefs->group_labels)
                    this->addGroupItem(++row, current);
                else
                    this->addGroupLabel(++row, current);
            }

            this->insertRow(++row);
            this->setRowHeight(row, prefs->row_height);
            QTableWidgetItem *index = new QTableWidgetItem(QString().number(GUID.at(idx)));
            this->setItem(row, 0, index);

            for (int col = 0; col < colCount; col++)
                if (this->CoverColumn-1 != col)
                    this->addRowItem(row, col, helper->parseLine(GUID.at(idx), prefs->rows_format.at(col)));

            prev = current;
        }
    }

    if (this->CoverColumn > -1)
        row = this->addCover(groupRow+1, row - groupRow, helper->filePath(GUID.last()));
}


int SimplePlaylist::addCover(int row, int spanRow, const QString &searchPath)
{
    const QBrush &brush = this->palette().brush(QPalette::Base);

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

}

void SimplePlaylist::defPlhighlight()
{

}
