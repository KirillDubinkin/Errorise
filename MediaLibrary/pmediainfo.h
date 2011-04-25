#ifndef PMEDIAINFO_H
#define PMEDIAINFO_H

#include "mediainfo.h"

#include <QObject>
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>

#include <QString>
#include <QStringList>
#include <QMultiMap>

class PMediaInfo : public MediaInfo
{
    Q_OBJECT
public:
    explicit PMediaInfo(QObject *parent = 0);

    void scanFile (QString filename);
    void scanFiles(QStringList files);
    void scanDir  (QString path);

    inline void scanForChanges() {}


signals:
    void fileScaned();

public slots:

private slots:
    void pringTags();
    void scanNextFile();
    void recursiveDirs(QString from);

private:
    Phonon::MediaObject *object;
    QStringList filenames;
    QString sourcePath;
    QMultiMap<QString, QMultiMap<QString, QString> > meta;
          //! filename, (TAG, Value);

};

#endif // PMEDIAINFO_H
