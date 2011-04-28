#ifndef _VERSION_H_
#define _VERSION_H_

#define NAME "Errorise"
#define VERSION "0.1.2.245"
#define COMPANY "we_apon"

#include <QString>
#include <QTime>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>

static inline QString myplayerVersion() { return QString(VERSION + QFileInfo(qApp->applicationFilePath()).lastModified().toString("   ddd, dd.MM.yyyy hh:mm") ); }
static inline QString myplayerName() { return QString(NAME); }
static inline QString mycompany() { return QString(COMPANY); }

#endif
