#include "simpleplaylistprefswidget.h"
#include "ui_simpleplaylistprefswidget.h"

#include "global.h"
#include <QListWidgetItem>

SimplePlaylistPrefsWidget::SimplePlaylistPrefsWidget(SimplePLPrefs *prefs, QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SimplePlaylistPrefsWidget)
{
    ui->setupUi(this);
    this->prefs = prefs;

    load();
}

SimplePlaylistPrefsWidget::~SimplePlaylistPrefsWidget()
{
    delete ui;
}


void SimplePlaylistPrefsWidget::load()
{
        //! General
    ui->alternateColorsBox->setChecked(prefs->alternate_colors);
    ui->showHeaderBox->setChecked(prefs->show_header);
    ui->styleEdit->setPlainText(prefs->stylesheet);
    ui->plArtSaveBox->setChecked(Global::pref->use_pl_art);
    ui->plArtSaveEdit->setText(Global::pref->pl_art_filename);
    fillArtSearchLists();


        //! Columns
    fillColNamesList();


        //! Groups
    ui->grpByDirs->setChecked(prefs->group_byDirs);
    ui->grpShowHeaderBox->setChecked(prefs->group_header);

    ui->grpHeigthEdit->setText(QString::number(prefs->group_height));
    ui->grpDelayEdit->setText(QString::number(prefs->group_delay));

    ui->grpColorBackEdit->setText(prefs->groups_back_color.name());
    ui->grpColorTextEdit->setText(prefs->groups_text_color.name());

    ui->grpTextEdit->setText(prefs->groups_format);
}


void SimplePlaylistPrefsWidget::fillArtSearchLists()
{
    ui->artFoldersList->clear();
    ui->artFilesList->clear();

    foreach(QString folder, Global::pref->art_search_folders)
        ui->artFoldersList->addItem(new QListWidgetItem(folder));

    foreach(QString file, Global::pref->art_search_patterns)
        ui->artFilesList->addItem(new QListWidgetItem(file));
}


void SimplePlaylistPrefsWidget::fillColNamesList()
{
    ui->colList->clear();

    foreach(QString column, prefs->columns_names)
        ui->colList->addItem(new QListWidgetItem(column));
}
