#ifndef MPLAYER_H
#define MPLAYER_H

#include "interfaces/playerinterface.h"
#include "mplayerprocess.h"

class Mplayer : public PlayerInterface
{
    Q_OBJECT
public:
    explicit Mplayer(QueueInterface *q = 0, QObject *parent = 0);

signals:

public slots:

};

#endif // MPLAYER_H
