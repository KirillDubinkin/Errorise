#include "simpleguiprefswidget.h"
#include "ui_simpleguiprefswidget.h"

SimpleGuiPrefsWidget::SimpleGuiPrefsWidget(SimpleGUIPrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpleGuiPrefsWidget)
{
    ui->setupUi(this);
    ui->titleText->setText(p->title_format);
    prefs = p;

    connect(ui->titleText, SIGNAL(textEdited(QString)), this, SLOT(changeWindowTitle(QString)));

    setAttribute(Qt::WA_DeleteOnClose);
}

SimpleGuiPrefsWidget::~SimpleGuiPrefsWidget()
{
    delete ui;
}
