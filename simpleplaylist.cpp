#include "simpleplaylist.h"
#include <QHeaderView>

SimplePlaylist::SimplePlaylist(bool showColumnsNames, QWidget *parent) :
    QTableWidget(parent)
{
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
                                        bool useLabels, bool useGroupLabels)
{
    // Clear
    this->setRowCount(0);
    int groupID = 0, row = 0, colCount = text[num].size();

    if (!useLabels){
        for (int idx = 0; idx < num; idx++){
            if (idx == groupIndex.at(groupID)){
                if (this->CoverColumn > -1){
//                   row = this->addCover(groupIndex.at(groupID)+1, row - groupIndex.at(groupID), QFileInfo(mediaInfo->track[idx-1].filename).dir());
                    row++;
                }

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
                if (this->CoverColumn > -1){
//                   row = this->addCover(groupIndex.at(groupID)+1, row - groupIndex.at(groupID), QFileInfo(mediaInfo->track[idx-1].filename).dir());
                    row++;
                }

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















