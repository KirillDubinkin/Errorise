#include "fileoperations.h"
#include "helper.h"

#include <QDebug>
#include <QMessageBox>
#include <QDir>


QStringList FileOperations::removeDialog(QList<int> ids)
{
    QStringList filepaths, filenames;

    foreach (int id, ids) {
        QString path = Helper::valueOfTrack("filepath", id);
        filepaths.append(path);
        filenames.append(path.mid(path.lastIndexOf("/") + 1));
    }

    QString dialogQuestion = tr("Are you sure want to remove those files permanently?") + "\n";

    if (filenames.size() < 6) {
        foreach (QString name, filenames)
            dialogQuestion.append("\n" + name);

    } else {
        for (int i = 0; i < 3; i++)
            dialogQuestion.append("\n" + filenames.at(i));
        dialogQuestion.append("\n...");
        dialogQuestion.append("\n" + filenames.last());
    }


    int btn = QMessageBox::warning(0, tr("Remove files from disk"), dialogQuestion,
                                    QMessageBox::Yes, QMessageBox::Cancel);

    if (btn == QMessageBox::Yes) {
        qDebug() << "Remove files from disk:\n\t" << filepaths.join("\n\t  ");
        return filepaths;
    }

    return QStringList();
}


QList<int> FileOperations::removeFiles(QList<int> ids)
{
    QList<int> removedFiles;

    if (ids.isEmpty())
        return removedFiles;

    QStringList filePaths = removeDialog(ids);
    if (filePaths.isEmpty())
        return removedFiles;

    QDir dir;

    for (int i = 0; i < filePaths.size(); i++)
        if (!dir.remove(filePaths.at(i)))
            qWarning() << "Can't remove file:" << filePaths.at(i);
        else
            removedFiles.append(ids.at(i));


    if (!removedFiles.isEmpty())
        Helper::removeTracks(removedFiles);

    return removedFiles;
}


bool FileOperations::moveFiles(QList<int> ids)
{
    if (ids.isEmpty())
        return false;

    qDebug() << "FileOperations::moveFiles" << ids;
    return true;
}
