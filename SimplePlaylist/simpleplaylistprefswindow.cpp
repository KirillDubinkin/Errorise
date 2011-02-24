#include "simpleplaylistprefswindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

SimplePlaylistPrefsWindow::SimplePlaylistPrefsWindow(SimplePLPrefs *preferences, QWidget *parent) :
    QTabWidget(parent)
{
    prefs = preferences;

    this->createGeneral();
}


void SimplePlaylistPrefsWindow::createGeneral()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QGridLayout *checkboxLayout = new QGridLayout();  // checkboxLayout

    showHeader = new QCheckBox(tr("Show Header"));
    showHeader->setChecked(prefs->show_header);

    alternateColors = new QCheckBox(tr("Alternate Colors"));
    alternateColors->setChecked(prefs->alternate_colors);

    checkboxLayout->addWidget(showHeader, 0, 0);
    checkboxLayout->addWidget(alternateColors, 0, 1);




    QLabel *styleLbl = new QLabel(tr("Palylist Stylesheet"));
    styleEdit = new QTextEdit(prefs->stylesheet);

    QVBoxLayout *styleLayout = new QVBoxLayout();
    styleLayout->addWidget(styleLbl);
    styleLayout->addWidget(styleEdit, 100);



    mainLayout->addLayout(checkboxLayout);
    mainLayout->addLayout(styleLayout, 100);

    QWidget *widget = new QWidget();
    widget->setLayout(mainLayout);
    this->addTab(widget, tr("General"));
}


