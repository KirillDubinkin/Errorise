#ifndef MPLAYER_H
#define MPLAYER_H

#include "interfaces/playerinterface.h"
#include "mplayerprocess.h"

#include "core.h" //! just for developing stage...

class Mplayer : public PlayerInterface
{
    Q_OBJECT
public:
    explicit Mplayer(QueueInterface *q = 0, QObject *parent = 0);

signals:

public slots:
    bool play(int guid = -1, int seek = -1);
    bool pause();
    bool playNext();
    bool playPrev();    //! Need to work
    bool stop();
    bool seek(int sec);

    bool restart();     //! And here too


private:
    Core *core;
    bool openNewFile(int guid, int seek);

};

#endif // MPLAYER_H
