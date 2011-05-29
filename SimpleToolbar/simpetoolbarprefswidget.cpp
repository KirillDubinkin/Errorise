#include "simpetoolbarprefswidget.h"
#include "ui_simpetoolbarprefswidget.h"

#include <QListWidgetItem>
#include <QFileDialog>
#include <QDebug>

SimpeToolbarPrefsWidget::SimpeToolbarPrefsWidget(SimpleToolbarPrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpeToolbarPrefsWidget)
{
    ui->setupUi(this);
    prefs = p;
    timer.setSingleShot(true);

    load();
    conct();

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

    ui->stylesheetEdit->setPlainText(prefs->style);

    connect(ui->toolList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen()));
}


void SimpeToolbarPrefsWidget::conct()
{
    connect(ui->widthLine,      SIGNAL(textChanged(QString)), this, SLOT(setToolWidth(QString)));
    connect(ui->heightLine,     SIGNAL(textChanged(QString)), this, SLOT(setToolHeight(QString)));
    connect(ui->textLine,       SIGNAL(textChanged(QString)), this, SLOT(setToolText(QString)));
    connect(ui->iconLine,       SIGNAL(textChanged(QString)), this, SLOT(setToolIcon(QString)));
    connect(ui->stylesheetEdit, SIGNAL(textChanged()),        this, SLOT(setToolStylesheet()));

    connect(this, SIGNAL(needTimer()), this, SLOT(startTimerNow()));
    connect(&timer, SIGNAL(timeout()), this, SIGNAL(somethingChanged()));
}


void SimpeToolbarPrefsWidget::itemChosen()
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


void SimpeToolbarPrefsWidget::setToolWidth(QString text)
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    bool ok;
    int width = text.toInt(&ok);

    if (ok)
    {
        int prevWidth;

        switch (tool)
        {
        case Seekbar:   break;
        case Volume:    break;
        case Play:      prevWidth = prefs->btnPlayWidth;      prefs->btnPlayWidth      = width; break;
        case Pause:     prevWidth = prefs->btnPauseWidth;     prefs->btnPauseWidth     = width; break;
        case PlayPause: prevWidth = prefs->btnPlayPauseWidth; prefs->btnPlayPauseWidth = width; break;
        case Stop:      prevWidth = prefs->btnStopWidth;      prefs->btnStopWidth      = width; break;
        case Prev:      prevWidth = prefs->btnPrevWidth;      prefs->btnPrevWidth      = width; break;
        case Next:      prevWidth = prefs->btnNextWidth;      prefs->btnNextWidth      = width; break;

        default:
            prevWidth = prefs->toolList.at(ui->toolList->currentRow()).toInt();

            prefs->toolList.replace(ui->toolList->currentRow(), QString::number(width));

            ui->toolList->item(ui->toolList->currentRow())->setText(
                    tr("Spacing") + " (" + QString::number(width) + ")");
            break;
        }

        if (width != prevWidth)
            emit needTimer();
    }
}


void SimpeToolbarPrefsWidget::setToolHeight(QString text)
{
    if (!text.isEmpty() && text.toInt() != prefs->btnHeight)
    {
        prefs->btnHeight = text.toInt();
        emit needTimer();
    }
}


void SimpeToolbarPrefsWidget::setToolText(QString text)
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    QString prevText;

    switch (tool)
    {
    case Seekbar:   break;
    case Volume:    break;
    case Play:      prevText = prefs->btnPlayText;      prefs->btnPlayText      = text; break;
    case Pause:     prevText = prefs->btnPauseText;     prefs->btnPauseText     = text; break;
    case PlayPause: prevText = prefs->btnPlayPauseText; prefs->btnPlayPauseText = text; break;
    case Stop:      prevText = prefs->btnStopText;      prefs->btnStopText      = text; break;
    case Prev:      prevText = prefs->btnPrevText;      prefs->btnPrevText      = text; break;
    case Next:      prevText = prefs->btnNextText;      prefs->btnNextText      = text; break;

    default:        break;
    }

    if (text != prevText)
        emit needTimer();
}


void SimpeToolbarPrefsWidget::setToolIcon(QString filename)
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    QString prevFilename;

    switch (tool)
    {
    case Seekbar:   break;
    case Volume:    break;
    case Play:      prevFilename = prefs->btnPlayIcon;      prefs->btnPlayIcon      = filename; break;
    case Pause:     prevFilename = prefs->btnPauseIcon;     prefs->btnPauseIcon     = filename; break;
    case PlayPause: prevFilename = prefs->btnPlayPauseIcon; prefs->btnPlayPauseIcon = filename; break;
    case Stop:      prevFilename = prefs->btnStopIcon;      prefs->btnStopIcon      = filename; break;
    case Prev:      prevFilename = prefs->btnPrevIcon;      prefs->btnPrevIcon      = filename; break;
    case Next:      prevFilename = prefs->btnNextIcon;      prefs->btnNextIcon      = filename; break;

    default:        break;
    }

    if (filename != prevFilename)
        emit needTimer();
}


void SimpeToolbarPrefsWidget::getToolIcon()
{
    QString filename = QFileDialog::getOpenFileName(this,
                tr("Select AlbumTree items icon..."), ui->iconLine->text(),
                tr("Images (*.svg *.png *.gif *.xpm *.jpg *.bmp)"), 0,
                QFileDialog::ReadOnly);

    if (!filename.isEmpty())
        ui->iconLine->setText(filename);
}


void SimpeToolbarPrefsWidget::setToolStylesheet()
{
    prefs->style = ui->stylesheetEdit->toPlainText();
}


void SimpeToolbarPrefsWidget::startTimerNow()
{
    timer.stop();

    timer.start(100);
}
