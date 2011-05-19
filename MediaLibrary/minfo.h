#ifndef MINFO_H
#define MINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMultiMap>
#include <QDir>

/*
    Recursive folders scan relised in MusicLibrary
    MusicLibrary will be send group of files to scanner
    And will be wait 'allFilesScanned' signal
*/

class Minfo: public QObject
{
    Q_OBJECT
protected:
    Minfo(QObject *parent = 0) : QObject(parent) { }


public slots:
    virtual void scanFiles  (QStringList files)   = 0;
    virtual void reScanFiles(QStringList files) = 0;

public:
    virtual void scanFile (QString filename)    = 0;
    virtual void scanDir  (QString path)        = 0;

    virtual QString findArt(QString path);
    virtual QString findPlArt(QDir dir);

signals:
    void newFilesScanned(QMultiMap<QString, QMultiMap<QString, QString> > tags);
    void oldFilesScanned(QMultiMap<QString, QMultiMap<QString, QString> > tags);

    void fileWasRemoved(int id);
    void fileWasUpdated(int id, QMultiMap<QString, QString> tags);
    void newFileAdded  (QString filename, QMultiMap<QString, QString> tags);
    void updateLibraryNow();

};

#endif // MINFO_H
