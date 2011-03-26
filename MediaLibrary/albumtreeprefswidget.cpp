#include "albumtreeprefswidget.h"
#include "ui_albumtreeprefswidget.h"

#include <QFileDialog>
#include <QTimer>

AlbumTreePrefsWidget::AlbumTreePrefsWidget(AlbumTreePrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumTreePrefsWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlags(Qt::Window);
    this->setGeometry(parent->x() - 100, parent->y() - 100, 640, 480);

    prefs = p;
    conct();
    load();

    timer.setSingleShot(true);
}


AlbumTreePrefsWidget::~AlbumTreePrefsWidget()
{
    delete ui;
}


void AlbumTreePrefsWidget::load()
{
        //! General
    ui->ItemIconLine->setText(prefs->items_icon);
    ui->TreePatternLine->setText(prefs->pattern);
    ui->StyleSheetEdit->setPlainText(prefs->stylesheet);
}


void AlbumTreePrefsWidget::conct()
{
    connect(ui->IconButtn,       SIGNAL(clicked()),            this, SLOT(getIcon()));
    connect(ui->ItemIconLine,    SIGNAL(textChanged(QString)), this, SLOT(setTreeItemIcon(QString)));
    connect(ui->TreePatternLine, SIGNAL(textEdited(QString)),  this, SLOT(setTreePattern(QString)));
    connect(ui->StyleSheetEdit,  SIGNAL(textChanged()),        this, SLOT(setTreeStyle()));
    connect(&timer,              SIGNAL(timeout()),            this, SLOT(emitStylesheetChange()));
}


void AlbumTreePrefsWidget::getIcon()
{
    QString filename = QFileDialog::getOpenFileName(this,
                tr("Select AlbumTree items icon..."), QDir::homePath(),
                tr("Images (*.svg *.png *.gif *.xpm *.jpg *.bmp)"), 0,
                QFileDialog::ReadOnly);

    if (!filename.isEmpty())
        ui->ItemIconLine->setText(filename);
}


void AlbumTreePrefsWidget::setTreeItemIcon(QString text)
{
    prefs->items_icon = text;
    ui->IconButtn->setIcon(QIcon(text));
    ui->IconButtn->setText("");

    emit iconChanged(text);
}


void AlbumTreePrefsWidget::setTreeStyle()
{
    if (timer.isActive())
    {
        timer.stop();
        prefs->stylesheet = ui->StyleSheetEdit->toPlainText();
        timer.start(1000);
    }
}
