#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QObject>
#include "queueinterface.h"

enum playerState { playerStoped = 0x1, playerPlayed = 0x2, playerPaused = 0x4 };

class PlayerInterface : public QObject
{
    Q_OBJECT
public:
    explicit PlayerInterface(QueueInterface * queue = 0, QObject *parent = 0);

signals:
    void state_changed(playerState state);
    void current_time_changed(int sec);
    void current_track_changed(int guid);
    void seeked(int sec);
    void stoped();
    void paused();

public slots:
    virtual bool play(int guid = -1, int seek = -1) = 0;
    virtual bool pause() = 0;
    virtual bool playNext() = 0;
    virtual bool playPrev() = 0;
    virtual bool stop() = 0;
    virtual bool seek(int sec) = 0;

    virtual bool restart() = 0;

public:
    virtual bool isPlaying() = 0;
    virtual bool isPaused() = 0;
    virtual int currentTrack() const = 0;  //! return guid
    virtual playerState currentState() = 0;

};

#endif // PLAYERINTERFACE_H