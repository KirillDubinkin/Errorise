#include "albumtreeprefs.h"

#include <QSettings>
#include <QDir>
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

const QString PlugName = "AlbumTree";

AlbumTreePrefs::AlbumTreePrefs()
{
    reset();
    load();
}


AlbumTreePrefs::~AlbumTreePrefs()
{
    save();
}


void AlbumTreePrefs::reset()
{
    pattern    = "%filepath%";
    stylesheet = "";
    items_icon = "";
}


void AlbumTreePrefs::load()
{
#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    set.beginGroup("General");
    pattern    = set.value("pattern", pattern).toString();
    stylesheet = set.value("stylesheet", stylesheet).toString();
    items_icon = set.value("items_icon", items_icon).toString();
    set.endGroup();
}


void AlbumTreePrefs::save()
{
#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    set.beginGroup("General");
    set.setValue("pattern", pattern);
    set.setValue("stylesheet", stylesheet);
    set.setValue("items_icon", items_icon);
    set.endGroup();

    set.sync();
}
