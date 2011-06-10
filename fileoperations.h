#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QList>
#include <QStringList>
#include <QWidget>
#include <QCoreApplication>

class FileOperations
{
    Q_DECLARE_TR_FUNCTIONS(FileOperations)

public:
    static QList<int> removeFiles(QList<int> ids);
    static bool moveFiles  (QList<int> ids);

private:
    FileOperations() {}
    static QStringList removeDialog(QList<int> ids);
};

#endif // FILEOPERATIONS_H
