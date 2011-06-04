#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>
#include <QList>
#include <QStringList>

class FileOperations : public QObject
{
    Q_OBJECT
public:
    explicit FileOperations(QObject *parent = 0);

signals:

public slots:
    static bool removeFiles(QList<int> ids) {}
    static bool removeFiles(QStringList filepaths) {}
    static bool removeDir(QString path) {}

};

#endif // FILEOPERATIONS_H
