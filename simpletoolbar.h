#ifndef SIMPLETOOLBAR_H
#define SIMPLETOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QMenu>
#include "myslider.h"

enum btns { Play = 1000, Pause = 1010,  PlayPause = 1020, Stop = 1030,
           Prev = 1040, Next = 1050 };

enum tools { Buttons = 5000, Seekbar = 5010, Volume = 5020,
             SmallMenu = 5030, FullMenu = 5040, PlaybackOrder = 5050 };

class ToolbarPrefs
{
public:
    ToolbarPrefs();
    ~ToolbarPrefs();

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



private:
    QString filename;
};



class SimpleToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleToolbar(QWidget *parent = 0);
    ~SimpleToolbar();

    MySlider *seekbar();
    MySlider *vol();

    QPushButton *btnPlay();
    QPushButton *btnNext();
    QPushButton *btnPrev();
    QPushButton *btnStop();
    QPushButton *btnPause();
    QPushButton *btnPlayPause();

private:
    void initComponents();

    QHBoxLayout *L;
    ToolbarPrefs *prefs;

    MySlider *seek_bar, *volume;
    QMenu *small_menu, *full_menu, *playback_order;
    QPushButton *btn_play, *btn_next, *btn_prev, *btn_stop, *btn_pause, *btn_play_pause;

signals:

public slots:
    void hideVol();
    void hideSeekbar();

};

#endif // SIMPLETOOLBAR_H
