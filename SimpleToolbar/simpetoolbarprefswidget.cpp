#include "simpetoolbarprefswidget.h"
#include "ui_simpetoolbarprefswidget.h"

SimpeToolbarPrefsWidget::SimpeToolbarPrefsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpeToolbarPrefsWidget)
{
    ui->setupUi(this);
}

SimpeToolbarPrefsWidget::~SimpeToolbarPrefsWidget()
{
    delete ui;
}
