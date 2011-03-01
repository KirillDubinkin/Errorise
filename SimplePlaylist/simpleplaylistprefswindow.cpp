#include "simpleplaylistprefswindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QDebug>

SimplePlaylistPrefsWindow::SimplePlaylistPrefsWindow(SimplePLPrefs *preferences, QWidget *parent) :
    QTabWidget(parent)
{
    prefs = preferences;

    titleFont.setBold(true);

    this->createGeneralTab();
}


void SimplePlaylistPrefsWindow::createGeneralTab()
{

//!  Checkboxes
    QGridLayout *checkboxLayout = new QGridLayout();  // checkboxLayout

    showHeader = new QCheckBox(tr("Show Header"));
    showHeader->setChecked(prefs->show_header);
    connect(showHeader, SIGNAL(toggled(bool)), this, SLOT(setShowHeader(bool)));

    alternateColors = new QCheckBox(tr("Alternate Colors"));
    alternateColors->setChecked(prefs->alternate_colors);
    connect(alternateColors, SIGNAL(toggled(bool)), this, SLOT(setAlternateColors(bool)));

    checkboxLayout->addWidget(showHeader, 0, 0);
    checkboxLayout->addWidget(alternateColors, 0, 1);
//! //////////////////////////////////////////////////////////




//!  Artwork
    artFolders = new QListWidget();
    artFolders->setEditTriggers(QAbstractItemView::DoubleClicked);
    fillList(artFolders, prefs->art_search_folders, Qt::ItemIsEditable);

    connect(artFolders, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(artFoldersChange(QListWidgetItem*)));



    artPatterns = new QListWidget();
    artPatterns->setEditTriggers(QAbstractItemView::DoubleClicked);
    fillList(artPatterns, prefs->art_search_pattern, Qt::ItemIsEditable);

    connect(artPatterns, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(artPatternsChange(QListWidgetItem*)));


    QLabel *artLabel = new QLabel(tr("Artwork Search"));
    artLabel->setFont(titleFont);

    QGridLayout *artLayout = new QGridLayout();
    artLayout->addWidget(artLabel, 0, 0, 1, 2);
    artLayout->addWidget(artFolders, 1, 0);
    artLayout->addWidget(artPatterns, 1, 1);
//! //////////////////////////////////////////////////////////




//!  Stylesheet
    QLabel *styleLbl = new QLabel(tr("Playlist Stylesheet"));
    styleLbl->setFont(titleFont);

    styleEdit = new QTextEdit(prefs->stylesheet);
    connect(styleEdit, SIGNAL(textChanged()), this, SLOT(setPlStyleheet()));

    QVBoxLayout *styleLayout = new QVBoxLayout();
    styleLayout->addWidget(styleLbl);
    styleLayout->addWidget(styleEdit, 100);
//! //////////////////////////////////////////////////////////




    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(checkboxLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(artLayout, 35);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(styleLayout, 100);

    QWidget *widget = new QWidget();
    widget->setLayout(mainLayout);
    this->addTab(widget, tr("General"));
}


void SimplePlaylistPrefsWindow::createColumsTab()
{

}


void SimplePlaylistPrefsWindow::setShowHeader(bool show)
{
    prefs->show_header = show;
    emit showHeaderChanged(show);
}

void SimplePlaylistPrefsWindow::setAlternateColors(bool b)
{
    prefs->alternate_colors = b;
    emit useAlternateColorsChanged(b);
}

void SimplePlaylistPrefsWindow::setPlStyleheet()
{
    prefs->stylesheet = styleEdit->toPlainText();
    emit styleEdited(prefs->stylesheet);
}

void SimplePlaylistPrefsWindow::artFoldersChange(QListWidgetItem *item)
{
    prefs->art_search_folders.replace(artFolders->row(item), item->text());
    emit artSearchChanged();
}

void SimplePlaylistPrefsWindow::artPatternsChange(QListWidgetItem *item)
{
    prefs->art_search_pattern.replace(artPatterns->row(item), item->text());
    emit artSearchChanged();
}

void SimplePlaylistPrefsWindow::fillList(QListWidget *list, const QStringList &strlist,
                                         Qt::ItemFlags flags)
{
    foreach(QString str, strlist)
    {
        QListWidgetItem *item = new QListWidgetItem(str, list);
        item->setFlags(item->flags() | flags);
    }
}
