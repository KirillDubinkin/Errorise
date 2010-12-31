#ifndef SIMPLETOOLBAR_H
#define SIMPLETOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "myslider.h"


class ToolbarPrefs
{
public:
    ToolbarPrefs();
    ~ToolbarPrefs();

    void reset();
    void load();
    void save();

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

    //! Other
    QString style;

private:
    QString filename;
};



class SimpleToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleToolbar(QWidget *parent = 0);
    ~SimpleToolbar();

    ToolbarPrefs *prefs;

    QHBoxLayout *L;
    MySlider *progress, *vol;
    QWidget *btns;
    QPushButton *btnPlay, *btnNext, *btnPrev, *btnStop, *btnPause, *btnPlayPause;

private:
    void createButtons();

signals:

public slots:

};

#endif // SIMPLETOOLBAR_H
