#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QObject>
#include "queueinterface.h"

enum playerState { playerStoped = 0x1, playerPlaying = 0x2, playerPaused = 0x4 };

class PlayerInterface : public QObject
{
    Q_OBJECT
public:
    inline explicit PlayerInterface(QueueInterface * q = 0, QObject *parent = 0)
        : QObject(parent)
    { queue = q; }

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
    inline  bool isPlaying()            { return (state != playerStoped); }
    inline  bool isPaused()             { return (state == playerPaused); }
    inline  playerState currentState()  { return state; }
    inline  int currentTrack() const    { return curTrack; }  //! return guid

protected:
    QueueInterface *queue;
    playerState state;
    int curTrack;


};

#endif // PLAYERINTERFACE_H
