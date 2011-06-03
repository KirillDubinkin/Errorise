#include "simpetoolbarprefswidget.h"
#include "ui_simpetoolbarprefswidget.h"

#include <QListWidgetItem>
#include <QFileDialog>
#include <QDebug>
#include <QAction>
#include <QMenu>

SimpeToolbarPrefsWidget::SimpeToolbarPrefsWidget(SimpleToolbarPrefs *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimpeToolbarPrefsWidget)
{
    ui->setupUi(this);
    prefs = p;
    timer.setSingleShot(true);

    load();
    conct();
    createToolListMenu();

    this->setWindowFlags(Qt::Window);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}


SimpeToolbarPrefsWidget::~SimpeToolbarPrefsWidget()
{
    delete ui;
}


void SimpeToolbarPrefsWidget::load()
{
    loadToolList();

    ui->stylesheetEdit->setPlainText(prefs->style);

    connect(ui->toolList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemChosen()));
}


void SimpeToolbarPrefsWidget::conct()
{
    connect(ui->widthLine,      SIGNAL(textEdited(QString)),  this, SLOT(setToolWidth(QString)));
    connect(ui->heightLine,     SIGNAL(textEdited(QString)),  this, SLOT(setToolHeight(QString)));
    connect(ui->textLine,       SIGNAL(textEdited(QString)),  this, SLOT(setToolText(QString)));
    connect(ui->iconLine,       SIGNAL(textChanged(QString)), this, SLOT(setToolIcon(QString)));
    connect(ui->stylesheetEdit, SIGNAL(textChanged()),        this, SLOT(setToolStylesheet()));
    connect(ui->iconButton,     SIGNAL(clicked()),            this, SLOT(getToolIcon()));

    connect(this, SIGNAL(needTimer()), this, SLOT(startTimerNow()));
    connect(&timer, SIGNAL(timeout()), this, SIGNAL(somethingChanged()));
}


void SimpeToolbarPrefsWidget::createToolListMenu()
{
    QMenu *menu, *chldmenu;

    ui->toolList->setContextMenuPolicy(Qt::ActionsContextMenu);

    menu = new QMenu(ui->toolList);

    chldmenu = menu->addMenu(tr("Add new tool"));
    chldmenu->addAction(tr("Spacing"),              this, SLOT(addSpacing()));
    chldmenu->addSeparator();
    chldmenu->addAction(tr("Seek bar"),             this, SLOT(addSeekBar()));
    chldmenu->addAction(tr("Volume bar"),           this, SLOT(addVolumeBar()));
    chldmenu->addSeparator();
    chldmenu->addAction(tr("Play button"),          this, SLOT(addButtonPlay()));
    chldmenu->addAction(tr("Pause button"),         this, SLOT(addButtonPause()));
    chldmenu->addAction(tr("Play or Pause button"), this, SLOT(addButtonPlayOrPause()));
    chldmenu->addAction(tr("Stop button"),          this, SLOT(addButtonStop()));
    chldmenu->addAction(tr("Previous button"),      this, SLOT(addButtonPrev()));
    chldmenu->addAction(tr("Next button"),          this, SLOT(addButtonNext()));

    menu->addSeparator();
    menu->addAction(tr("Move Up"),     this, SLOT(moveUp()));
    menu->addAction(tr("Move down"),   this, SLOT(moveDown()));
    menu->addSeparator();
    menu->addAction(tr("Remove item"), this, SLOT(removeTool()));

    ui->toolList->addActions(menu->actions());
}


void SimpeToolbarPrefsWidget::addSeekBar()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Seekbar)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Seekbar));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addSpacing()
{
    prefs->toolList.insert(ui->toolList->currentRow()+1, "6");
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addVolumeBar()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Volume)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Volume));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonPlay()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Play)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Play));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonPause()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Pause)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Pause));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonPlayOrPause()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::PlayPause)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::PlayPause));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonStop()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Stop)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Stop));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonPrev()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Prev)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Prev));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::addButtonNext()
{
    if (prefs->toolList.contains(QString::number(SimpleToolbarPrefs::Next)))
        return;

    prefs->toolList.insert(ui->toolList->currentRow()+1, QString::number(SimpleToolbarPrefs::Next));
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::removeTool()
{
    if ((ui->toolList->currentRow() > -1) && (ui->toolList->currentRow() < prefs->toolList.size()))
    {
        prefs->toolList.removeAt(ui->toolList->currentRow());
        loadToolList();

        emit needTimer();
    }
}


void SimpeToolbarPrefsWidget::moveUp()
{
    if (!ui->toolList->currentRow())
        return;

    prefs->toolList.move(ui->toolList->currentRow(), ui->toolList->currentRow() - 1);
    loadToolList();
    emit needTimer();
}


void SimpeToolbarPrefsWidget::moveDown()
{
    if (ui->toolList->currentRow() >= prefs->toolList.size() - 1)
        return;

    prefs->toolList.move(ui->toolList->currentRow(), ui->toolList->currentRow() + 1);
    loadToolList();
    emit needTimer();
}



void SimpeToolbarPrefsWidget::loadToolList()
{
    ui->toolList->clear();

    for (int i = 0; i < prefs->toolList.size(); i++)
    {
        switch (QString(prefs->toolList.at(i)).toInt())
        {
        case SimpleToolbarPrefs::Seekbar: ui->toolList->addItem(new QListWidgetItem(tr("Seek bar"))); break;
        case SimpleToolbarPrefs::Volume: ui->toolList->addItem(new QListWidgetItem(tr("Volume"))); break;
        case SimpleToolbarPrefs::Play: ui->toolList->addItem(new QListWidgetItem(tr("Play button"))); break;
        case SimpleToolbarPrefs::Pause: ui->toolList->addItem(new QListWidgetItem(tr("Pause button"))); break;
        case SimpleToolbarPrefs::PlayPause: ui->toolList->addItem(new QListWidgetItem(tr("Play or pause button"))); break;
        case SimpleToolbarPrefs::Stop: ui->toolList->addItem(new QListWidgetItem(tr("Stop button"))); break;
        case SimpleToolbarPrefs::Prev: ui->toolList->addItem(new QListWidgetItem(tr("Previous button"))); break;
        case SimpleToolbarPrefs::Next: ui->toolList->addItem(new QListWidgetItem(tr("Next button"))); break;

        default: ui->toolList->addItem(new QListWidgetItem(tr("Spacing") + " (" + prefs->toolList.at(i) + ")")); break;
        }
    }
}

void SimpeToolbarPrefsWidget::itemChosen()
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    switch (tool)
    {
    case SimpleToolbarPrefs::Seekbar:   enableAll(false); clearFields(); break;
    case SimpleToolbarPrefs::Volume:    enableAll(false); clearFields(); break;
    case SimpleToolbarPrefs::Play:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPlayWidth));
        ui->textLine->setText(prefs->btnPlayText);
        ui->iconLine->setText(prefs->btnPlayIcon);
        break;
    case SimpleToolbarPrefs::Pause:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPauseWidth));
        ui->textLine->setText(prefs->btnPauseText);
        ui->iconLine->setText(prefs->btnPauseIcon);
        break;
    case SimpleToolbarPrefs::PlayPause:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPlayPauseWidth));
        ui->textLine->setText(prefs->btnPlayPauseText);
        ui->iconLine->setText(prefs->btnPlayPauseIcon);
        break;
    case SimpleToolbarPrefs::Stop:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnStopWidth));
        ui->textLine->setText(prefs->btnStopText);
        ui->iconLine->setText(prefs->btnStopIcon);
        break;
    case SimpleToolbarPrefs::Prev:
        enableAll(true);
        ui->heightLine->setText(QString::number(prefs->btnHeight));
        ui->widthLine->setText(QString::number(prefs->btnPrevWidth));
        ui->textLine->setText(prefs->btnPrevText);
        ui->iconLine->setText(prefs->btnPrevIcon);
        break;
    case SimpleToolbarPrefs::Next:
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

    bool ok = true;
    int width;

    if (text.isEmpty())
        width = 0;
    else
        width = text.toInt(&ok);

    if (ok)
    {
        int prevWidth = -1;

        switch (tool)
        {
        case SimpleToolbarPrefs::Seekbar:   break;
        case SimpleToolbarPrefs::Volume:    break;
        case SimpleToolbarPrefs::Play:      prevWidth = prefs->btnPlayWidth;      prefs->btnPlayWidth      = width; break;
        case SimpleToolbarPrefs::Pause:     prevWidth = prefs->btnPauseWidth;     prefs->btnPauseWidth     = width; break;
        case SimpleToolbarPrefs::PlayPause: prevWidth = prefs->btnPlayPauseWidth; prefs->btnPlayPauseWidth = width; break;
        case SimpleToolbarPrefs::Stop:      prevWidth = prefs->btnStopWidth;      prefs->btnStopWidth      = width; break;
        case SimpleToolbarPrefs::Prev:      prevWidth = prefs->btnPrevWidth;      prefs->btnPrevWidth      = width; break;
        case SimpleToolbarPrefs::Next:      prevWidth = prefs->btnNextWidth;      prefs->btnNextWidth      = width; break;

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
    bool ok = true;
    int height;

    if (text.isEmpty())
        height = 0;
    else
        height = text.toInt(&ok);

    if (ok)
    {
        prefs->btnHeight = height;
        emit needTimer();
    }
}


void SimpeToolbarPrefsWidget::setToolText(QString text)
{
    int tool = QString(prefs->toolList.at(ui->toolList->currentRow())).toInt();

    QString prevText;

    switch (tool)
    {
    case SimpleToolbarPrefs::Seekbar:   break;
    case SimpleToolbarPrefs::Volume:    break;
    case SimpleToolbarPrefs::Play:      prevText = prefs->btnPlayText;      prefs->btnPlayText      = text; break;
    case SimpleToolbarPrefs::Pause:     prevText = prefs->btnPauseText;     prefs->btnPauseText     = text; break;
    case SimpleToolbarPrefs::PlayPause: prevText = prefs->btnPlayPauseText; prefs->btnPlayPauseText = text; break;
    case SimpleToolbarPrefs::Stop:      prevText = prefs->btnStopText;      prefs->btnStopText      = text; break;
    case SimpleToolbarPrefs::Prev:      prevText = prefs->btnPrevText;      prefs->btnPrevText      = text; break;
    case SimpleToolbarPrefs::Next:      prevText = prefs->btnNextText;      prefs->btnNextText      = text; break;

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
    case SimpleToolbarPrefs::Seekbar:   break;
    case SimpleToolbarPrefs::Volume:    break;
    case SimpleToolbarPrefs::Play:      prevFilename = prefs->btnPlayIcon;      prefs->btnPlayIcon      = filename; break;
    case SimpleToolbarPrefs::Pause:     prevFilename = prefs->btnPauseIcon;     prefs->btnPauseIcon     = filename; break;
    case SimpleToolbarPrefs::PlayPause: prevFilename = prefs->btnPlayPauseIcon; prefs->btnPlayPauseIcon = filename; break;
    case SimpleToolbarPrefs::Stop:      prevFilename = prefs->btnStopIcon;      prefs->btnStopIcon      = filename; break;
    case SimpleToolbarPrefs::Prev:      prevFilename = prefs->btnPrevIcon;      prefs->btnPrevIcon      = filename; break;
    case SimpleToolbarPrefs::Next:      prevFilename = prefs->btnNextIcon;      prefs->btnNextIcon      = filename; break;

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
    {
        ui->iconLine->setText(filename);
        emit needTimer();
    }
}


void SimpeToolbarPrefsWidget::setToolStylesheet()
{
    if (prefs->style != ui->stylesheetEdit->toPlainText())
    {
        prefs->style = ui->stylesheetEdit->toPlainText();
        emit styleChanged(prefs->style);
    }
}


void SimpeToolbarPrefsWidget::startTimerNow()
{
    timer.stop();

    timer.start(400);
}
