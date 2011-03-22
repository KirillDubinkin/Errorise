#ifndef SIMPLETOOLBAR_H
#define SIMPLETOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QMenu>

#include <Phonon/VolumeSlider>
#include <Phonon/SeekSlider>

#include <SimpleToolbar/simpletoolbarprefs.h>

class SimpleToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleToolbar(QWidget *parent = 0);
    ~SimpleToolbar();

//    MySlider *seekbar();

    Phonon::VolumeSlider *vol();
    Phonon::SeekSlider   *seekbar();

    QPushButton *btnPlay();
    QPushButton *btnNext();
    QPushButton *btnPrev();
    QPushButton *btnStop();
    QPushButton *btnPause();
    QPushButton *btnPlayPause();

private:
    void initComponents();

    QHBoxLayout *L;
    SimpleToolbarPrefs *prefs;

   // MySlider *seek_bar, *volume;



    Phonon::VolumeSlider *volume;
    Phonon::SeekSlider   *seek_bar;

    QMenu *small_menu, *full_menu, *playback_order;
    QPushButton *btn_play, *btn_next, *btn_prev, *btn_stop, *btn_pause, *btn_play_pause;

signals:

public slots:
    void hideVol();
    void hideSeekbar();

};

#endif // SIMPLETOOLBAR_H
