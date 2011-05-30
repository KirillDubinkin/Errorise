#include "simpleplaylistprefswidget.h"
#include "ui_simpleplaylistprefswidget.h"

#include "global.h"
#include <QListWidgetItem>
#include <QDebug>

SimplePlaylistPrefsWidget::SimplePlaylistPrefsWidget(SimplePLPrefs *prefs, QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SimplePlaylistPrefsWidget)
{
    ui->setupUi(this);
    this->prefs = prefs;

    load();
    conct();
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
    ui->plArtSaveEdit->setEnabled(ui->plArtSaveBox->isChecked());
    fillArtSearchLists();


        //! Columns
    fillColNamesList();
    connect(ui->colList, SIGNAL(currentRowChanged(int)), this, SLOT(columnChosen(int)));
    if (!prefs->columns_names.isEmpty()) ui->colList->setCurrentRow(0);


        //! Groups
    ui->grpByDirs->setChecked(prefs->group_byDirs);
    ui->grpShowHeaderBox->setChecked(prefs->group_header);

    ui->grpHeigthEdit->setText(QString::number(prefs->group_height));
    ui->grpDelayEdit->setText(QString::number(prefs->group_delay));

    ui->grpColorBackEdit->setText(prefs->groups_back_color.name());
    ui->grpColorTextEdit->setText(prefs->groups_text_color.name());
    ui->grpTextEdit->setText(prefs->groups_format);
    setAlignBoxState(ui->grpAlignBox, prefs->groups_text_aligment);
}


void SimplePlaylistPrefsWidget::conct()
{
        //! General
    connect(ui->alternateColorsBox, SIGNAL(toggled(bool)), this, SLOT(changeAlternateColors(bool)));
    connect(ui->showHeaderBox,      SIGNAL(toggled(bool)), this, SLOT(changeHeaderVisible(bool)));
    connect(ui->styleEdit,          SIGNAL(textChanged()), this, SLOT(changeStylesheet()));
    connect(ui->plArtSaveBox,       SIGNAL(toggled(bool)), this, SLOT(changePlArtSave(bool)));
    connect(ui->plArtSaveEdit,      SIGNAL(textChanged(QString)), this, SLOT(changePlArtFilename(QString)));
}


void SimplePlaylistPrefsWidget::changePlArtFilename(QString filename)
{
    Global::pref->pl_art_filename = filename;
}


void SimplePlaylistPrefsWidget::changeStylesheet()
{
    prefs->stylesheet = ui->styleEdit->toPlainText();
    emit stylesheetChanged(prefs->stylesheet);
}


void SimplePlaylistPrefsWidget::changePlArtSave(bool enable)
{
    Global::pref->use_pl_art = enable;
    ui->plArtSaveEdit->setEnabled(enable);
}


void SimplePlaylistPrefsWidget::changeHeaderVisible(bool enable)
{
    prefs->show_header = enable;
    emit headerVisibleChanged(enable);
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


void SimplePlaylistPrefsWidget::columnChosen(int col)
{
    ui->colWidth->setText(QString::number(prefs->columns_sizes.at(col)));
    ui->colHeight->setText(QString::number(prefs->row_height));
    ui->colColorEdit->setText(prefs->rows_text_color.at(col));
    setAlignBoxState(ui->colAlignBox, prefs->columns_aligment.at(col));
}


void SimplePlaylistPrefsWidget::setAlignBoxState(QComboBox *box, int alignment)
{
    switch (alignment) {
    case Qt::AlignLeft:    box->setCurrentIndex(0); break;
    case Qt::AlignRight:   box->setCurrentIndex(1); break;
    case Qt::AlignHCenter: box->setCurrentIndex(2); break;
    case Qt::AlignJustify: box->setCurrentIndex(3); break;
    default: qWarning() << "SimplePlaylistPrefsWidget::setAlignBoxState()\n\t Undefined aligment:" << alignment; break;
    }
}


void SimplePlaylistPrefsWidget::changeAlternateColors(bool enable)
{
    prefs->alternate_colors = enable;
    emit alternateColorsChanged(enable);
}


