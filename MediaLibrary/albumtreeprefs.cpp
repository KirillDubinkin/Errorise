#include "albumtreeprefs.h"

#include "paths.h"
#include <QSettings>
#include <QDir>
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

const QString PlugName = "AlbumTree";

AlbumTreePrefs::AlbumTreePrefs()
{
    filename = Paths::iniPath() + QDir::separator() + PlugName + ".ini";

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

    header     = PlugName;
    showHeader = false;
}


void AlbumTreePrefs::load()
{
#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QString(QApplication::applicationName()).toLower(), PlugName);
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, "", QString(QApplication::applicationName()).toLower(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    qDebug() << "name" << set.fileName();


    set.beginGroup("General");
    pattern    = set.value("pattern", pattern).toString();
    stylesheet = set.value("stylesheet", stylesheet).toString();
    items_icon = set.value("items_icon", items_icon).toString();
    set.endGroup();


    set.beginGroup("Header");
    header     = set.value("header", header).toString();
    showHeader = set.value("showHeader", showHeader).toBool();
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


    set.beginGroup("Header");
    set.setValue("header", header);
    set.setValue("showHeader", showHeader);
    set.endGroup();


    set.sync();
}
