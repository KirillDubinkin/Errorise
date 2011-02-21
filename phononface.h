#ifndef PHONONFACE_H
#define PHONONFACE_H

#include <QObject>
#include "interfaces/playerinterface.h"

#include <Phonon/MediaObject>
#include <Phonon/MediaSource>
#include <Phonon/AudioOutput>
#include <Phonon/Path>


class PhononFace : public QObject
{
    Q_OBJECT
public:
    explicit PhononFace(QObject *parent = 0);

    bool    isSekable();
    QString currentTrack();
    int     currentGuid();

    inline Phonon::AudioOutput * audioOutput() { return output;  }
    inline Phonon::MediaObject * mediaObject() { return mobject; }

signals:
    void aboutToFinish();
    void finished();
    void tick(qint64 tick);
    void trackChanged(QString filename, int guid);
    void needNext();


public slots:
    void play(int guid = -1);
    void pause();
    void stop();
    void seek(int sec);   //! If need a seek with slider, use Phonon::SeekSlider widget
    void next();
    void prev();

    void enqueue(int guid);
    void enqueue(QList<int> guids);

    void printTick(qint64 tick);

private:
    Phonon::MediaObject *mobject;
    Phonon::AudioOutput *output;
    Phonon::Path         path;

    QList<Phonon::MediaSource> prevTracks;
    bool isPreviousPlaying;

    Phonon::MediaSource        currentSource;
        //! It's changes on trackChanged() signal,
        //! that emited in sourceChange() function.
        //! So, it can be used in sourceChange(), like 100% previous source.


private slots:
    void sourceChange(Phonon::MediaSource);

};

#endif // PHONONFACE_H
