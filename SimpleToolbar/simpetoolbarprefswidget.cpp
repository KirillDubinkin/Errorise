#include "simpetoolbarprefswidget.h"
#include "ui_simpetoolbarprefswidget.h"

#include <QListWidgetItem>

SimpeToolbarPrefsWidget::SimpeToolbarPrefsWidget(SimpleToolbarPrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpeToolbarPrefsWidget)
{
    ui->setupUi(this);
    prefs = p;
    load();

    this->setWindowFlags(Qt::Window);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}


SimpeToolbarPrefsWidget::~SimpeToolbarPrefsWidget()
{
    delete ui;
}


void SimpeToolbarPrefsWidget::load()
{
    for (int i = 0; i < prefs->toolList.size(); i++)
    {
        switch (QString(prefs->toolList.at(i)).toInt())
        {
        case Seekbar: ui->toolList->addItem(new QListWidgetItem(tr("Seekbar"))); break;
        case Volume: ui->toolList->addItem(new QListWidgetItem(tr("Volume"))); break;
        case Play: ui->toolList->addItem(new QListWidgetItem(tr("Play button"))); break;
        case Pause: ui->toolList->addItem(new QListWidgetItem(tr("Pause button"))); break;
        case PlayPause: ui->toolList->addItem(new QListWidgetItem(tr("Play or pause button"))); break;
        case Stop: ui->toolList->addItem(new QListWidgetItem(tr("Stop button"))); break;
        case Prev: ui->toolList->addItem(new QListWidgetItem(tr("Previous button"))); break;
        case Next: ui->toolList->addItem(new QListWidgetItem(tr("Next button"))); break;

        default: ui->toolList->addItem(new QListWidgetItem(tr("Spacing") + " (" + prefs->toolList.at(i) + ")")); break;
        }
    }

    connect(ui->toolList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen(QListWidgetItem*)));
}


void SimpeToolbarPrefsWidget::itemChosen(QListWidgetItem *item)
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    switch (tool)
    {
    case Seekbar:   enableAll(false); clearFields(); break;
    case Volume:    enableAll(false); clearFields(); break;
    case Play:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPlayWidth));
        ui->textLine->setText(prefs->btnPlayText);
        ui->iconLine->setText(prefs->btnPlayIcon);
        break;
    case Pause:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPauseWidth));
        ui->textLine->setText(prefs->btnPauseText);
        ui->iconLine->setText(prefs->btnPauseIcon);
        break;
    case PlayPause:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPlayPauseWidth));
        ui->textLine->setText(prefs->btnPlayPauseText);
        ui->iconLine->setText(prefs->btnPlayPauseIcon);
        break;
    case Stop:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnStopWidth));
        ui->textLine->setText(prefs->btnStopText);
        ui->iconLine->setText(prefs->btnStopIcon);
        break;
    case Prev:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPrevWidth));
        ui->textLine->setText(prefs->btnPrevText);
        ui->iconLine->setText(prefs->btnPrevIcon);
        break;
    case Next:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnNextWidth));
        ui->textLine->setText(prefs->btnNextText);
        ui->iconLine->setText(prefs->btnNextIcon);
        break;

    default:
        enableAll(false);
        clearFields();
        ui->widthLabel->setEnabled(true);
        ui->widthLine->setEnabled(true);
        ui->widthLine->setText(QString::number(tool));
        break;
    }
}


void SimpeToolbarPrefsWidget::enableAll(bool enable)
{
    enableSize(enable);
    enableText(enable);
    enableIcons(enable);
}


void SimpeToolbarPrefsWidget::enableIcons(bool enable)
{
    ui->iconButton->setEnabled(enable);
    ui->iconLabel->setEnabled(enable);
    ui->iconLine->setEnabled(enable);
}


void SimpeToolbarPrefsWidget::enableSize(bool enable)
{
    ui->widthLabel->setEnabled(enable);
    ui->widthLine->setEnabled(enable);
    ui->heightLabel->setEnabled(enable);
    ui->heightLine->setEnabled(enable);
}


void SimpeToolbarPrefsWidget::enableText(bool enable)
{
    ui->textLabel->setEnabled(enable);
    ui->textLine->setEnabled(enable);
}


void SimpeToolbarPrefsWidget::clearFields()
{
    ui->widthLine->clear();
    ui->heightLine->clear();
    ui->textLine->clear();
    ui->iconLine->clear();
}
