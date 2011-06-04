#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QWidget>

class FileOperations : public QObject
{
    Q_OBJECT
public:
    explicit FileOperations(QObject *parent = 0);

signals:

public slots:
    static bool removeFiles(QList<int> ids, QWidget *parent = 0);
    static bool moveFiles  (QList<int> ids, QWidget *parent = 0);

};

#endif // FILEOPERATIONS_H
