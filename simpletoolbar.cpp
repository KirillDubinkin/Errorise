#include "simpletoolbar.h"
#include "paths.h"
#include <QSettings>
#include <QTextCodec>
#include <QDebug>

#define PLUG_NAME "SimpleToolbar"


ToolbarPrefs::ToolbarPrefs()
{
    filename = Paths::iniPath() + "/" + PLUG_NAME + ".ini";
    reset();
    load();
}

ToolbarPrefs::~ToolbarPrefs()
{
    save();
}

void ToolbarPrefs::reset()
{
    qDebug("SimpleToolbar->Prefs::reset()");

    //! Buttons
    btnHeight = 21;
    btnsList << "1004" << "1003" << "1005" << "1006";

    btnPlayIcon = "";
    btnPlayText = "[|>]";
    btnPlayWidth = 34;

    btnStopIcon = "";
    btnStopText = "[#]";
    btnStopWidth = 34;

    btnNextIcon = "";
    btnNextText = "[>>|]";
    btnNextWidth = 34;

    btnPrevIcon = "";
    btnPrevText = "[|<<]";
    btnPrevWidth = 34;

    btnPauseIcon = "";
    btnPauseText = "[||]";
    btnPauseWidth = 34;

    btnPlayPauseIcon = "";
    btnPlayPauseText = "[|> | ||]";
    btnPlayPauseWidth = 50;


    //! Other
    style = "";
}

void ToolbarPrefs::save()
{
    qDebug("SimpleToolbar->Prefs::save()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Buttons
    set.beginGroup("Buttons");
    set.setValue("btnHeight", btnHeight);
    set.setValue("btnsList", btnsList);

    set.setValue("btnPlayIcon", btnPlayIcon);
    set.setValue("btnPlayText", btnPlayText);
    set.setValue("btnPlayWidth", btnPlayWidth);

    set.setValue("btnStopIcon", btnStopIcon);
    set.setValue("btnStopText", btnStopText);
    set.setValue("btnStopWidth", btnStopWidth);

    set.setValue("btnNextIcon", btnNextIcon);
    set.setValue("btnNextText", btnNextText);
    set.setValue("btnNextWidth", btnNextWidth);

    set.setValue("btnPrevIcon", btnPrevIcon);
    set.setValue("btnPrevText", btnPrevText);
    set.setValue("btnPrevWidth", btnPrevWidth);

    set.setValue("btnPauseIcon", btnPauseIcon);
    set.setValue("btnPauseText", btnPauseText);
    set.setValue("btnPauseWidth", btnPauseWidth);

    set.setValue("btnPlayPauseIcon", btnPlayPauseIcon);
    set.setValue("btnPlayPauseText", btnPlayPauseText);
    set.setValue("btnPlayPauseWidth", btnPlayPauseWidth);

    set.endGroup();


    //! Other
    set.beginGroup("Other");
    set.setValue("style", style);
    set.endGroup();

    set.sync();
}

void ToolbarPrefs::load()
{
    qDebug("SimpleToolbar->Prefs::load()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Buttons
    set.beginGroup("Buttons");
    btnHeight = set.value("btnHeight", btnHeight).toInt();
    btnsList = set.value("btnsList", btnsList).toStringList();

    btnPlayIcon = set.value("btnPlayIcon", btnPlayIcon).toString();
    btnPlayText = set.value("btnPlayText", btnPlayText).toString();
    btnPlayWidth = set.value("btnPlayWidth", btnPlayWidth).toInt();

    btnStopIcon = set.value("btnStopIcon", btnStopIcon).toString();
    btnStopText = set.value("btnStopText", btnStopText).toString();
    btnStopWidth = set.value("btnStopWidth", btnStopWidth).toInt();

    btnNextIcon = set.value("btnNextIcon", btnNextIcon).toString();
    btnNextText = set.value("btnNextText", btnNextText).toString();
    btnNextWidth = set.value("btnNextWidth", btnNextWidth).toInt();

    btnPrevIcon = set.value("btnPrevIcon", btnPrevIcon).toString();
    btnPrevText = set.value("btnPrevText", btnPrevText).toString();
    btnPrevWidth = set.value("btnPrevWidth", btnPrevWidth).toInt();

    btnPauseIcon = set.value("btnPauseIcon", btnPauseIcon).toString();
    btnPauseText = set.value("btnPauseText", btnPauseText).toString();
    btnPauseWidth = set.value("btnPauseWidth", btnPauseWidth).toInt();

    btnPlayPauseIcon = set.value("btnPlayPauseIcon", btnPlayPauseIcon).toString();
    btnPlayPauseText = set.value("btnPlayPauseText", btnPlayPauseText).toString();
    btnPlayPauseWidth = set.value("btnPlayPauseWidth", btnPlayPauseWidth).toInt();

    set.endGroup();


    //! Other
    set.beginGroup("Other");
    style = set.value("style", style).toString();
    set.endGroup();
}


SimpleToolbar::SimpleToolbar(QWidget *parent) :
    QWidget(parent)
{
    prefs = new ToolbarPrefs();

    this->setStyleSheet(prefs->style);

    progress = new MySlider();

    vol = new MySlider();
    vol->setMinimum(0);
    vol->setMaximum(100);

    this->createButtons();


    L = new QHBoxLayout;
    L->addWidget(btns);
    L->addWidget(progress, 100);
    L->addWidget(vol);

    this->setLayout(L);

}

SimpleToolbar::~SimpleToolbar()
{
    delete prefs;
}

void SimpleToolbar::createButtons()
{
    btns = new QWidget;

    btnPlay = new QPushButton(QIcon(prefs->btnPlayIcon), prefs->btnPlayText);
    btnPlay->setMaximumSize(prefs->btnPlayWidth, prefs->btnHeight);
    btnPlay->setMinimumSize(btnPlay->maximumSize());

    btnStop = new QPushButton(QIcon(prefs->btnStopIcon), prefs->btnStopText);
    btnStop->setMaximumSize(prefs->btnStopWidth, prefs->btnHeight);
    btnStop->setMinimumSize(btnStop->maximumSize());

    btnNext = new QPushButton(QIcon(prefs->btnNextIcon), prefs->btnNextText);
    btnNext->setMaximumSize(prefs->btnNextWidth, prefs->btnHeight);
    btnNext->setMinimumSize(btnNext->maximumSize());

    btnPrev = new QPushButton(QIcon(prefs->btnPrevIcon), prefs->btnPrevText);
    btnPrev->setMaximumSize(prefs->btnPrevWidth, prefs->btnHeight);
    btnPrev->setMinimumSize(btnPrev->maximumSize());

    btnPause = new QPushButton(QIcon(prefs->btnPauseIcon), prefs->btnPauseText);
    btnPause->setMaximumSize(prefs->btnPauseWidth, prefs->btnHeight);
    btnPause->setMinimumSize(btnPause->maximumSize());

    btnPlayPause = new QPushButton(QIcon(prefs->btnPlayPauseText), prefs->btnPlayPauseText);
    btnPlayPause->setMaximumSize(prefs->btnPlayPauseWidth, prefs->btnHeight);
    btnPlayPause->setMinimumSize(btnPlayPause->maximumSize());



    QHBoxLayout *l = new QHBoxLayout;
    for (int i = 0; i < prefs->btnsList.size(); i++)
    {
        switch (QString(prefs->btnsList.at(i)).toInt())
        {
            case Play: l->addWidget(this->btnPlay); break;
            case Pause: l->addWidget(this->btnPause); break;
            case PlayPause: l->addWidget(this->btnPlayPause); break;
            case Stop: l->addWidget(this->btnStop); break;
            case Prev: l->addWidget(this->btnPrev); break;
            case Next: l->addWidget(this->btnNext); break;
            default: l->addSpacing(QString(prefs->btnsList.at(i)).toInt());
        }
    }

    btns->setLayout(l);

}
