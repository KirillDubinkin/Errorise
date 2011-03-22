#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <QString>
#include <QTranslator>

class Translator
{

public:
	Translator();
	~Translator();

	void load(QString locale);

protected:
	static bool loadCatalog(QTranslator & t, QString name, QString locale, QString dir);

	QTranslator app_trans;
	QTranslator qt_trans;
};

#endif
