#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    load();

    setGeometry(parent->x() + (parent->width() / 2 - 360),
                parent->y() + (parent->height() / 2 - 240),
                720, 480);

    setWindowFlags(Qt::Dialog);
}

About::~About()
{
    delete ui;
}


void About::load()
{
    setWindowTitle(tr("About") + " " + qApp->applicationName());
    ui->lnlIcon->setPixmap(QPixmap(":icons/about.png"));
    ui->lnlAppName->setText(qApp->applicationName() + "  v" + qApp->applicationVersion());
}
