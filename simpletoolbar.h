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
    QString btnPlayIcon;
    QString btnPlayText;

    QString btnStopIcon;
    QString btnStopText;

    QString btnNextIcon;
    QString btnNextText;

    QString btnPrevIcon;
    QString btnPrevText;


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
    QPushButton *btnPlay, *btnNext, *btnPrev, *btnStop;

private:
    void createButtons();

signals:

public slots:

};

#endif // SIMPLETOOLBAR_H
