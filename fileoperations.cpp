#include "fileoperations.h"

#include <QDebug>

FileOperations::FileOperations(QObject *parent) :
    QObject(parent)
{
}


bool FileOperations::removeFiles(QList<int> ids, QWidget *parent)
{
    if (ids.isEmpty())
        return false;

    qDebug() << "FileOperations::removeFiles" << ids;
    return true;
}


bool FileOperations::moveFiles(QList<int> ids, QWidget *parent)
{
    if (ids.isEmpty())
        return false;

    qDebug() << "FileOperations::moveFiles" << ids;
    return true;
}
