#ifndef PMEDIAINFO_H
#define PMEDIAINFO_H

#include "minfo.h"

#include <QObject>
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>

#include <QString>
#include <QStringList>
#include <QMultiMap>

class PMediaInfo : public Minfo
{
    Q_OBJECT
public:
    explicit PMediaInfo(QObject *parent = 0);

    void scanFile (QString filename);
    void scanDir  (QString path);

    void checkFiles(QStringList files);


public slots:
    void scanFiles  (QStringList files);
    void reScanFiles(QStringList files);


signals:
    void fileScaned();

private slots:
    void pringTags();
    void scanNextFile();
    void recursiveDirs(QString from);

private:
    Phonon::MediaObject *object;
    QString             artFilePath;
    QString             playlistArtFilePath;
    QStringList         filenames;
    QString             sourcePath;

    QMultiMap<QString, QMultiMap<QString, QString> > meta;
          //! filename, (TAG, Value);

    bool isUpdateState;

};

#endif // PMEDIAINFO_H
