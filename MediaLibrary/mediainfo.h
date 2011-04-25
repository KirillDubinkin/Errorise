#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMultiMap>

class MediaInfo: public QObject
{
    Q_OBJECT

public:
    inline MediaInfo(QObject *parent = 0) : QObject(parent) { }

    virtual void scanFile (QString filename)  = 0;
    virtual void scanFiles(QStringList files) = 0;
    virtual void scanDir  (QString path)      = 0;

    virtual void scanForChanges()             = 0;

signals:
    void allFilesScanned(QMultiMap<QString, QMultiMap<QString, QString> > tags);

    void fileWasRemoved(int id);
    void fileWasUpdated(int id, QMultiMap<QString, QString> tags);
    void newFileAdded  (QString filename, QMultiMap<QString, QString> tags);
    void updateLibraryNow();

};

#endif // MEDIAINFO_H
