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

    this->createGeneral();
}


void SimplePlaylistPrefsWindow::createGeneral()
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
    artFolders->addItems(prefs->art_search_folders);

    artPatterns = new QListWidget();
    artPatterns->addItems(prefs->art_search_pattern);

    QLabel *artLable = new QLabel(tr("Artwork Search"));
    artLable->setFont(titleFont);

    QGridLayout *artLayout = new QGridLayout();
    artLayout->addWidget(artLable, 0, 0, 1, 2);
    artLayout->addWidget(artFolders, 1, 0);
    artLayout->addWidget(artPatterns, 1, 1);
//! //////////////////////////////////////////////////////////




//!  Stylesheet
    QLabel *styleLbl = new QLabel(tr("Playlist Stylesheet"));
    styleLbl->setFont(titleFont);

    styleEdit = new QTextEdit(prefs->stylesheet);

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
