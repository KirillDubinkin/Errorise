#include "pmediainfo.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTimer>

PMediaInfo::PMediaInfo(QObject *parent) :
    QObject(parent)
{
    object = new Phonon::MediaObject(this);
    connect(object, SIGNAL(metaDataChanged()), this, SLOT(pringTags()));
    connect(this, SIGNAL(fileScaned()), this, SLOT(scanNextFile()));
}

void PMediaInfo::scanFile(QString filename)
{
    object->setCurrentSource(Phonon::MediaSource(filename));
}

void PMediaInfo::scanFiles(QStringList files)
{
    filenames = files;
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
    qDebug() << object->metaData("TRACKNUMBER") << object->metaData("TITLE");
    emit fileScaned();
}


void PMediaInfo::scanNextFile()
{
    if (filenames.size())
        scanFile(filenames.takeFirst());
}
