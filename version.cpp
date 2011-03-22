#include "version.h"

#define NAME "Errorise"
#define VERSION "0.1.2"
#define COMPANY "we_apon"

QString myplayerVersion() {
	return QString(VERSION);
}

QString myplayerName() {
    return QString(NAME);
}

QString mycompany() {
    return QString(COMPANY);
}
