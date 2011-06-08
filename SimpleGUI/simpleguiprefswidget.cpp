#include "simpleguiprefswidget.h"
#include "ui_simpleguiprefswidget.h"
#include "global.h"

#include <QStringList>
#include <QDir>

SimpleGuiPrefsWidget::SimpleGuiPrefsWidget(SimpleGUIPrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleGuiPrefsWidget)
{
    ui->setupUi(this);
    prefs = p;

    load();
    conct();

    setAttribute(Qt::WA_DeleteOnClose);
}

SimpleGuiPrefsWidget::~SimpleGuiPrefsWidget()
{
    delete ui;
}


void SimpleGuiPrefsWidget::load()
{
    ui->titleText->setText(prefs->title_format);
    fillLanguageBox();
}


void SimpleGuiPrefsWidget::conct()
{
    connect(ui->titleText, SIGNAL(textEdited(QString)), this, SLOT(changeWindowTitle(QString)));
    connect(ui->langBox,   SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
}


void SimpleGuiPrefsWidget::fillLanguageBox()
{
    QStringList translations;
    translations.append(tr("System"));
    translations.append("English");
    translations.append("ru_RU");



    QString trPath = Global::pref->configPath() + "/translations";
    QStringList trFiles = QDir(trPath).entryList(QStringList("errorise_*.qm"));

    foreach(QString file, trFiles)
    {
        file = file.mid(file.indexOf("_") + 1, file.indexOf(".qm"));

        if (!translations.contains(file))
            translations.append(file);
    }


    foreach(QString translation, translations)
    {
        if (translation == "ru_RU")
            ui->langBox->addItem("Russian", translation);
        else if (translation == "en_EN")
            ui->langBox->addItem("English", translation);

           // ... and so on ...

        else
            ui->langBox->addItem(translation, translation);
    }


    if (Global::pref->language.isEmpty())
        ui->langBox->setCurrentIndex(0);
    else
        ui->langBox->setCurrentIndex(translations.indexOf(Global::pref->language));

    ui->lblNeedRestart->clear();
}


void SimpleGuiPrefsWidget::changeLanguage(int boxIndex)
{
    QString lang = ui->langBox->itemData(boxIndex).toString();

    if (lang == tr("System"))
        Global::pref->language.clear();
    else
        Global::pref->language = lang;

    ui->lblNeedRestart->setText(tr("You must restart application to change language"));
}
