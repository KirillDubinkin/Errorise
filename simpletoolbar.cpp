#include "simpletoolbar.h"
#include "paths.h"
#include <QSettings>
#include <QTextCodec>

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
    btnPlayIcon = "";
    btnPlayText = "[|>]";

    btnStopIcon = "";
    btnStopText = "[#]";

    btnNextIcon = "";
    btnNextText = "[>>|]";

    btnPrevIcon = "";
    btnPrevText = "[|<<]";
}

void ToolbarPrefs::save()
{
    qDebug("SimpleToolbar->Prefs::save()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());

    //! Buttons
    set.beginGroup("Buttons");
    set.setValue("btnPlayIcon", btnPlayIcon);
    set.setValue("btnPlayText", btnPlayText);
    set.setValue("btnStopIcon", btnStopIcon);
    set.setValue("btnStopText", btnStopText);
    set.setValue("btnNextIcon", btnNextIcon);
    set.setValue("btnNextText", btnNextText);
    set.setValue("btnPrevIcon", btnPrevIcon);
    set.setValue("btnPrevText", btnPrevText);
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
    btnPlayIcon = set.value("btnPlayIcon", btnPlayIcon).toString();
    btnPlayText = set.value("btnPlayText", btnPlayText).toString();
    btnStopIcon = set.value("btnStopIcon", btnStopIcon).toString();
    btnStopText = set.value("btnStopText", btnStopText).toString();
    btnNextIcon = set.value("btnNextIcon", btnNextIcon).toString();
    btnNextText = set.value("btnNextText", btnNextText).toString();
    btnPrevIcon = set.value("btnPrevIcon", btnPrevIcon).toString();
    btnPrevText = set.value("btnPrevText", btnPrevText).toString();
    set.endGroup();
}


SimpleToolbar::SimpleToolbar(QWidget *parent) :
    QWidget(parent)
{
    prefs = new ToolbarPrefs();

    progress = new MySlider();

    vol = new MySlider();
    vol->setMinimum(0);
    vol->setMaximum(100);

    L = new QHBoxLayout;
    L->addWidget(progress, 100);
    L->addWidget(vol);

}

SimpleToolbar::~SimpleToolbar()
{
    delete prefs;
}

void SimpleToolbar::createButtons()
{
    btns = new QWidget;

    if (!prefs->btnPlayIcon.isEmpty())
        this->btnPlay = new QPushButton(QIcon(prefs->btnPlayIcon), prefs->btnPlayText);
    else
        this->btnPlay = new QPushButton(prefs->btnPlayText);

    if (!prefs->btnStopIcon.isEmpty())
        this->btnStop = new QPushButton(QIcon(prefs->btnStopIcon), prefs->btnStopText);
    else
        this->btnStop = new QPushButton(prefs->btnStopText);

    if (!prefs->btnNextIcon.isEmpty())
        this->btnNext = new QPushButton(QIcon(prefs->btnNextIcon), prefs->btnNextText);
    else
        this->btnNext = new QPushButton(prefs->btnNextText);


    //! Энамерик с названием кнопок в настройках, чтобы определить последовательность
    //! создавать все кнопки, но показывть лишь определённые в настройках
    //! или не создавать, но инициализировать указатели нулем, дабы избежать ексепшенов...
    //! добавить к настройкам каждой кнопки расстояние слева/справа


    QHBoxLayout *l = new QHBoxLayout;


}
