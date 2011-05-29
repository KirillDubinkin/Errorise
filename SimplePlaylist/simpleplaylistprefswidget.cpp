#include "simpleplaylistprefswidget.h"
#include "ui_simpleplaylistprefswidget.h"

SimplePlaylistPrefsWidget::SimplePlaylistPrefsWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SimplePlaylistPrefsWidget)
{
    ui->setupUi(this);
}

SimplePlaylistPrefsWidget::~SimplePlaylistPrefsWidget()
{
    delete ui;
}
