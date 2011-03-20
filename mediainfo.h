#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QList>
#include "mediadata.h"

class MediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit MediaInfo(QObject *parent = 0);
    ~MediaInfo();

    static int timeToSec(QString time);

    MediaData *track;  //! Like a DataBase

    int numParsedFiles;

signals:
    void newTracksReceived(const QList<int> &GUID);
    void stdoutParsed();
    void filesGetted();

public slots:
    void scanDir(const QString &spath);

private slots:
    void getFiles();
    void startScan();
    void readStdOut(int, QProcess::ExitStatus exitStatus);
    void parceStdOut();
    void takeGuids();

private:
    QProcess *minfo;
    QString path;

    void recursiveDirs(const QString &sDir);

    QStringList out; //! stdout
    QString line;
    QStringList files;

    QList<int> guid;  //! it allways contain a list of currently-used guids

    void parseDir();
    void parse(const QStringList &out, const QStringList &files);
    bool restarting;


};

#endif // MEDIAINFO_H
