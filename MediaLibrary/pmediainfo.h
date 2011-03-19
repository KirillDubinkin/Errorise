#ifndef PMEDIAINFO_H
#define PMEDIAINFO_H

#include <QObject>
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>

#include <QString>
#include <QStringList>

class PMediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit PMediaInfo(QObject *parent = 0);

    void scanFile (QString filename);
    void scanFiles(QStringList files);
    void scanDir  (QString path);


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

};

#endif // PMEDIAINFO_H
