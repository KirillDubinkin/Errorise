#include "pmediainfo.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTimer>
#include <QTime>

PMediaInfo::PMediaInfo(QObject *parent) :
    QObject(parent)
{
    object = new Phonon::MediaObject(this);

//    connect(object, SIGNAL(metaDataChanged()), this, SLOT(pringTags()));
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
    filenames = files;
    meta.clear();
    scanNextFile();
}


void PMediaInfo::scanDir(QString path)
{
    sourcePath = path;
    QTimer::singleShot(0, this, SLOT(recursiveDirs()));
}


void PMediaInfo::recursiveDirs(QString from)
{
    QDir dir(from);
    QFileInfoList list = dir.entryInfoList(QStringList() << "*.mp3",
                              QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    filenames.clear();

    foreach (QFileInfo info, list)
    {
        if (info.isDir())
            recursiveDirs(info.filePath());
        else
            filenames.append(info.filePath());
    }

    qDebug() << "end;";
}



void PMediaInfo::pringTags()
{
    if ((object->state() == Phonon::StoppedState)
        | (object->state() == Phonon::ErrorState))
    {
      //  qDebug() << object->metaData();

        QMultiMap<QString, QString> temp = object->metaData();
        QStringList format = object->currentSource().fileName().split(".");

        temp.insert("DURATION", QTime(0, 0).addMSecs(object->totalTime()).toString("mm:ss"));
        temp.insert("FORMAT", QString(format.last()).toUpper());

        meta.insert(object->currentSource().fileName(), temp);

        emit fileScaned();
    }
}


void PMediaInfo::scanNextFile()
{
    if (filenames.size())
        scanFile(filenames.takeFirst());
    else
        emit allFilesScanned(meta);
}
