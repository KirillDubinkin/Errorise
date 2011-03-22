#ifndef _VERSION_H_
#define _VERSION_H_

#define NAME "Errorise"
#define VERSION "0.1.2"
#define COMPANY "we_apon"

#include <QString>

static inline QString myplayerVersion() { return QString(VERSION); }
static inline QString myplayerName() { return QString(NAME); }
static inline QString mycompany() { return QString(COMPANY); }

#endif
