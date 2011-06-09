#include "pmediainfo.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTimer>
#include <QTime>

PMediaInfo::PMediaInfo(QObject *parent) : Minfo(parent)
{
    qDebug("Load PMediaInfo");
    object = new Phonon::MediaObject(this);

    connect(object, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(pringTags()));
    connect(this, SIGNAL(fileScaned()), this, SLOT(scanNextFile()));

}

void PMediaInfo::scanFile(QString filename)
{
    object->setCurrentSource(Phonon::MediaSource(filename));
}

void PMediaInfo::scanFiles(QStringList files)
{
    isUpdateState = false;

    qDebug() << "PMediaInfo::scanFiles" << files.size() << "\n\n" << files.join("\n") << "\n\n\n";

    playlistArtFilePath.clear();
    artFilePath.clear();

    playlistArtFilePath = findPlArt(QFileInfo(files.first()).absoluteDir());

    if (playlistArtFilePath.isEmpty())
        artFilePath     = findArt(QFileInfo(files.first()).absolutePath());


    filenames = files;
    meta.clear();
    scanNextFile();
}



void PMediaInfo::reScanFiles(QStringList files)
{
    isUpdateState = true;

    qDebug() << "PMediaInfo::reScanFiles" << files.size() << "\n\n" << files.join("\n") << "\n\n\n";

    playlistArtFilePath.clear();
    artFilePath.clear();

    playlistArtFilePath = findPlArt(QFileInfo(files.first()).absoluteDir());
    if (playlistArtFilePath.isEmpty())
        artFilePath     = findArt(QFileInfo(files.first()).absolutePath());

    filenames = files;
    meta.clear();
    scanNextFile();
}


void PMediaInfo::pringTags()
{
    if (object->state() == Phonon::ErrorState)
    {
        qWarning() << "PMediaInfo got error!\n"
                   << "\ttype:" << object->errorType()
                   << "\tmessage:" << object->errorString()
                   << "\tfile:" << object->currentSource().fileName() << endl;
    }

    if ((object->state() == Phonon::StoppedState)
        | (object->state() == Phonon::ErrorState))
    {

        QMultiMap<QString, QString> temp = object->metaData();
        QStringList format = object->currentSource().fileName().split(".");

        temp.insert("ART", artFilePath);
        temp.insert("PLAYLISTART", playlistArtFilePath);
        temp.insert("DURATION", QTime(0, 0).addMSecs(object->totalTime()).toString("mm:ss"));
        temp.insert("MODIFIED", QString::number(QFileInfo(format.join(".")).lastModified().toMSecsSinceEpoch()));

        fixMetadata(object->currentSource().fileName(), &temp);

        meta.insert(object->currentSource().fileName(), temp);

        emit fileScaned();
    }
}


void PMediaInfo::scanNextFile()
{
    if (filenames.size())
        scanFile(filenames.takeFirst());
    else
    {
        if (isUpdateState)
            emit oldFilesScanned(meta);
        else
            emit newFilesScanned(meta);
    }
}
