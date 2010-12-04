#ifndef SIMPLEPLAYLIST_H
#define SIMPLEPLAYLIST_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDir>
#include <QStringList>

class SimplePlaylist : public QTableWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylist(bool showColumnsNames = false, QWidget *parent = 0);

    int coverColumn();
    int lengthColumn();

signals:

public slots:    
    void setColumns(const QStringList &names, const QStringList &sizes, const QStringList &rowsFormat);
    void setTracks(const QStringList &text, const int &num, bool useLabels = false);
    void setTracksWithGroups(const QStringList &text, const int &num,
                             const QStringList &group, const QList<int> &groupIndex,
                             bool useLabels = false, bool useGroupLabels = false);


    void highlightCurrentTrack();
    void defPlhighlight();

private:
    QTableWidgetItem *newItem(const QBrush &background, Qt::ItemFlags flags = Qt::ItemIsEnabled,
                              const QString &text = "");
    void addRowItem(int idx, int row, int col);
    void addRowLabel(int idx, int row, int col);

    void addGroupItem(int row, const QString &text);
    void addGroupLabel(int row, const QString &text);

    void addCover(int row, int spanRow, const QDir &path);

    int CoverColumn;
    int LengthColumn;
};

#endif // SIMPLEPLAYLIST_H
