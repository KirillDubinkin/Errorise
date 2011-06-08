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
    save();
}


AlbumTreePrefs::~AlbumTreePrefs()
{
    save();
}


void AlbumTreePrefs::reset()
{
    pattern       = "%filepath%";
    stylesheet    = "AlbumTree {\n\tbackground-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(9, 14, 17, 255), stop:1 rgba(50, 57, 64, 255));\n\tcolor: rgb(200,200,200);\n\tgridline-color: rgb(74, 91, 108);\n\tselection-color: rgb(157, 196, 255);\n\tselection-background-color: rgb(67, 82, 98);\n}";
    items_icon    = "";
    context_icons = true;
}


void AlbumTreePrefs::load()
{
#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QApplication::applicationName().toLower(), PlugName);
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationName(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    set.beginGroup("General");
    pattern       = set.value("pattern", pattern).toString();
    stylesheet    = set.value("stylesheet", stylesheet).toString();
    items_icon    = set.value("items_icon", items_icon).toString();
    context_icons = set.value("context_icons", context_icons).toBool();
    set.endGroup();
}


void AlbumTreePrefs::save()
{
#ifdef Q_OS_LINUX
    QSettings set(QSettings::NativeFormat, QSettings::UserScope, QApplication::applicationName().toLower(), PlugName);
#else
    QSettings set(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationName(), PlugName);
#endif

    set.setIniCodec(QTextCodec::codecForLocale());

    set.beginGroup("General");
    set.setValue("pattern", pattern);
    set.setValue("stylesheet", stylesheet);
    set.setValue("items_icon", items_icon);
    set.setValue("context_icons", context_icons);
    set.endGroup();

    set.sync();
}
