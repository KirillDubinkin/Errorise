#include "simpleguiprefs.h"
#include <QSettings>
#include <QApplication>
#include <QTextCodec>

const QString PlugName = "SimpleGUI";

SimpleGUIPrefs::SimpleGUIPrefs()
{
    reset();
    load();
}

SimpleGUIPrefs::~SimpleGUIPrefs()
{
    save();
}


void SimpleGUIPrefs::reset()
{
    geometry.setRect(100, 100, 980, 576);
    title_format = "%artist% - [%date%] %title%";
}



void SimpleGUIPrefs::load()
{
#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());


    set.beginGroup("General");
    geometry = set.value("geometry", geometry).toRect();
    title_format = set.value("title_format", title_format).toString();
    set.endGroup();
}


void SimpleGUIPrefs::save()
{
#ifdef Q_OS_LINUX
        QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
        QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    set.beginGroup("General");
    set.setValue("geometry", geometry);
    set.setValue("title_format", title_format);
    set.endGroup();

    set.sync();
}

