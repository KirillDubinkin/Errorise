#include "fileoperations.h"
#include "helper.h"

#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

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


    int btn = QMessageBox::warning(0, qApp->applicationName() + ": " + tr("Remove files from disk"),
                                   dialogQuestion, QMessageBox::Yes, QMessageBox::Cancel);

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


bool FileOperations::moveFile(QString filepath, QString destPath)
{
    if (!QFile::exists(filepath))
        return false;

    if (QFile::rename(filepath, destPath))
        return true;

    qDebug() << "Can't rename. Will be copy & delete";

    QFile file(filepath);
    QFile destFile(destPath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Can't read existing file!");
        return false;
    }

    if (!destFile.open(QIODevice::WriteOnly)) {
        qWarning("Can't open destination file!");
        file.close();
        return false;
    }

    if (destFile.write(file.readAll()) == -1) {
        qWarning("Can't write destination file!");
        file.close();
        destFile.close();
        return false;
    }

    if (destFile.flush()) {
        destFile.close();
        if (file.remove())
            return true;

        qWarning("Can't remove source file!");
        return false;
    }

    qWarning("Can't flush destination file!");
    return false;
}


QList<int> FileOperations::moveFiles(QList<int> ids)
{
    if (ids.isEmpty())
        return ids;

    QStringList filePaths;
    foreach (int id, ids)
        filePaths.append(Helper::valueOfTrack("filepath", id));


    QList<int> movedFiles;

    QString dest = QFileDialog::getExistingDirectory(0,
                    qApp->applicationName() + " moves files: " + tr("Choose destination directory"),
                    filePaths.first().mid(0, filePaths.first().lastIndexOf("/")));

    if (dest.isEmpty())
        return movedFiles;


    qDebug() << "Moving files to" << dest;

    for (int i = 0; i < filePaths.size(); i++)
    {
        QString file = filePaths.at(i);

        if (!moveFile(file, dest + "/" + file.mid(file.lastIndexOf("/") + 1)))
            qWarning() << "Can't move file:" << file;
        else
            movedFiles.append(ids.at(i));
    }


    return movedFiles;
}
