#ifndef SIMPLETOOLBAR_H
#define SIMPLETOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QStringList>

#include <Phonon/VolumeSlider>
#include <Phonon/SeekSlider>

#include <SimpleToolbar/simpletoolbarprefs.h>
#include "SimpleToolbar/simpetoolbarprefswidget.h"

class SimpleToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleToolbar(QWidget *parent = 0);
    ~SimpleToolbar();

    Phonon::VolumeSlider *vol();
    Phonon::SeekSlider   *seekbar();

    QPushButton *btnPlay();
    QPushButton *btnNext();
    QPushButton *btnPrev();
    QPushButton *btnStop();
    QPushButton *btnPause();
    QPushButton *btnPlayPause();

    QWidget *getPrefsWidget();


public slots:
    void hideVol();
    void hideSeekbar();

private slots:
    inline void deletePreferences() { prefsWidget = 0; }
    void buildToolbar();

private:
    void initComponents();
    void deleteComponents();
    void createMenu();


private:
    QHBoxLayout *L;

    SimpleToolbarPrefs      *prefs;
    SimpeToolbarPrefsWidget *prefsWidget;

    Phonon::VolumeSlider *volume;
    Phonon::SeekSlider   *seek_bar;

    QPushButton *btn_play, *btn_next, *btn_prev, *btn_stop, *btn_pause, *btn_play_pause;

signals:
    void needPrefWindow();


};

#endif // SIMPLETOOLBAR_H
