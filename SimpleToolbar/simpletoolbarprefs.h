#ifndef SIMPLETOOLBARPREFS_H
#define SIMPLETOOLBARPREFS_H

#include <QString>
#include <QStringList>


class SimpleToolbarPrefs
{
public:

    enum btns { Play = 1000, Pause = 1010,  PlayPause = 1020, Stop = 1030,
               Prev = 1040, Next = 1050 };

    enum tools { Buttons = 5000, Seekbar = 5010, Volume = 5020,
                 SmallMenu = 5030, FullMenu = 5040, PlaybackOrder = 5050 };


    SimpleToolbarPrefs();
    ~SimpleToolbarPrefs();

    void reset();
    void load();
    void save();


    //! General
    QString style;
    QStringList toolList;


    //! Buttons
    int btnHeight;

    QString btnPlayIcon;
    QString btnPlayText;
    int btnPlayWidth;

    QString btnStopIcon;
    QString btnStopText;
    int btnStopWidth;

    QString btnNextIcon;
    QString btnNextText;
    int btnNextWidth;

    QString btnPrevIcon;
    QString btnPrevText;
    int btnPrevWidth;

    QString btnPauseIcon;
    QString btnPauseText;
    int btnPauseWidth;

    QString btnPlayPauseIcon;
    QString btnPlayPauseText;
    int btnPlayPauseWidth;

};


#endif // SIMPLETOOLBARPREFS_H
