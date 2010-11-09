#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "mediasettings.h"
#include "mplayerprocess.h"
#include "mediadata.h"


class MplayerProcess;
class QSettings;



class Core : public QObject
{
    Q_OBJECT
public:
     Core(QObject *parent = 0);
     ~Core();

    MediaData mdat;
    MediaSettings mset;

    bool playing;
    bool restarting;


protected:
    void playNewFile(QString file, int seek=-1);
    void initPlaying(int seek=-1);
    void startMplayer(QString file, double seek = -1 );
    void stopMplayer();

signals:
    void showTime();
    void finished();
    void playnext();


protected:
    MplayerProcess * proc;

public slots:
    void openFile(QString filename, int seek=-1);
    void tellmp(const QString & command);
    void pause();
    void stop();
    void goToSec(int sec);
    void setVolume(int volume);

protected slots:
    void changeCurrentSec(int sec);
    void processFinished();

};

#endif // CORE_H
