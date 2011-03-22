#include "simpletoolbarprefs.h"

#include <QSettings>
#include <QTextCodec>

#include "paths.h"

const QString PlugName = "SimpleToolbar";


SimpleToolbarPrefs::SimpleToolbarPrefs()
{
    filename = Paths::iniPath() + "/" + PlugName + ".ini";
    reset();
    load();
}


SimpleToolbarPrefs::~SimpleToolbarPrefs()
{
    save();
}


void SimpleToolbarPrefs::reset()
{
    //qDebug("SimpleToolbarPrefs::reset()");

    //! Buttons
    btnHeight = 21;

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
    toolList << QString::number(Stop) << QString::number(PlayPause) << QString::number(Prev)
            << QString::number(Next) << QString::number(Seekbar) << QString::number(Volume);

}


void SimpleToolbarPrefs::save()
{
    //qDebug("SimpleToolbar->Prefs::save()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());


    //! General
    set.beginGroup("General");
    set.setValue("style", style);
    set.setValue("toolList", toolList);
    set.endGroup();


    //! Buttons
    set.beginGroup("Buttons");
    set.setValue("btnHeight", btnHeight);

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


    set.sync();

}


void SimpleToolbarPrefs::load()
{
    //qDebug("SimpleToolbar->Prefs::load()");

    QSettings set(filename, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForLocale());


    //! General
    set.beginGroup("General");
    style = set.value("style", style).toString();
    toolList = set.value("toolList", toolList).toStringList();
    set.endGroup();


    //! Buttons
    set.beginGroup("Buttons");
    btnHeight = set.value("btnHeight", btnHeight).toInt();

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

}

