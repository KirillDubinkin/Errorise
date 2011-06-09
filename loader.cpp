#include "loader.h"
#include "global.h"
#include "SimpleGUI/simplegui.h"
#include <QTranslator>
#include <QApplication>
#include <QString>

Loader::Loader(QObject *parent): QObject(parent)
{
    gui_p = 0;
    Global::init_preferences();
    loadTranslations();
    Global::global_init();
}


Loader::~Loader()
{
    delete gui_p;
    Global::global_end();
}


bool Loader::loadTranslations()
{
    QString lang = QLocale::system().name();

    if (!Global::pref->language.isEmpty())
        lang = pref->language;

    if (lang == "English")
        return false;


    QString trFileName = "errorise_" + lang;
    QString trPath     = Global::pref->configPath() + "/translations";
    bool isLoad = false;

    QTranslator *appTranslator = new QTranslator(qApp);


    if (QFile::exists(trPath + "/" + trFileName + ".qm"))
        isLoad = appTranslator->load(trFileName, trPath);

    else {
        qDebug("Translation file \"%s\" not found\n\tusing translation from resource",
               QString(trPath + "/" + trFileName + ".qm").toUtf8().data());

        isLoad = appTranslator->load(trFileName, ":/translations");
    }


    if (isLoad) {
        qApp->installTranslator(appTranslator);
        qDebug("Translator successfully loaded!");
        return true;
    }


    qWarning("Translations for locale \"%s\" not found!", lang.toUtf8().data());
    delete appTranslator;
    return false;
}


QWidget * Loader::gui()
{
    if (!gui_p)
    {
        Global::gui = new SimpleGUI;
        gui_p = Global::gui;
    }

    return gui_p;
}


void Loader::start()
{
    gui()->show();
    //! I suppose that someday there will be several GUIs
    //! and function gui() will be decide which show
}


void Loader::processArgs(QStringList args)
{
    qDebug() << args;
    // TODO
}
