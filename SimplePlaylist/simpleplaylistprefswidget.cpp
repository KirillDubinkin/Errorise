#include "simpleplaylistprefswidget.h"
#include "ui_simpleplaylistprefswidget.h"

#include "global.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QColorDialog>
#include <QStringList>
#include <QAction>

SimplePlaylistPrefsWidget::SimplePlaylistPrefsWidget(SimplePLPrefs *prefs, QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SimplePlaylistPrefsWidget)
{
    ui->setupUi(this);
    this->prefs = prefs;
    editedCol = -1;

    load();
    conct();
    createContextMenus();

    if (!prefs->columns_names.isEmpty()) ui->colList->setCurrentRow(0);
    setCurrentIndex(prefs->prefs_tab);
}


SimplePlaylistPrefsWidget::~SimplePlaylistPrefsWidget()
{
    prefs->prefs_tab = currentIndex();
    delete ui;
}


void SimplePlaylistPrefsWidget::load()
{
        //! General
    ui->alternateColorsBox->setChecked(prefs->alternate_colors);
    ui->showHeaderBox->setChecked(prefs->show_header);
    ui->styleEdit->setPlainText(prefs->stylesheet);
    ui->plArtSaveBox->setChecked(Global::pref->use_pl_art);
    ui->plArtSaveEdit->setText(Global::pref->pl_art_filename);
    ui->plArtSaveEdit->setEnabled(ui->plArtSaveBox->isChecked());
    ui->showGridBox->setChecked(prefs->show_grid);
    ui->copyPatternEdit->setText(prefs->copy_preset);
    setGridStyleBoxState(prefs->grid_style);
    fillArtSearchLists();


        //! Columns
    fillColNamesList();
    connect(ui->colList, SIGNAL(currentRowChanged(int)), this, SLOT(columnChosen(int)));


        //! Groups
    ui->grpByDirs->setChecked(prefs->group_byDirs);
    ui->grpShowHeaderBox->setChecked(prefs->group_header);

    ui->grpHeigthEdit->setText(QString::number(prefs->group_height));
    ui->grpDelayEdit->setText(QString::number(prefs->group_delay));

    ui->grpColorBackEdit->setText(prefs->groups_back_color.name());
    ui->grpColorTextEdit->setText(prefs->groups_text_color.name());
    ui->grpTextEdit->setText(prefs->groups_format);
    ui->grpVATextEdit->setText(prefs->groups_va_format);
    setAlignBoxState(ui->grpAlignBox, prefs->groups_text_aligment);
}


void SimplePlaylistPrefsWidget::conct()
{
        //! General
    connect(ui->alternateColorsBox, SIGNAL(toggled(bool)), this, SLOT(changeAlternateColors(bool)));
    connect(ui->showHeaderBox,      SIGNAL(toggled(bool)), this, SLOT(changeHeaderVisible(bool)));
    connect(ui->showGridBox,        SIGNAL(toggled(bool)), this, SLOT(changeGridVisible(bool)));
    connect(ui->styleEdit,          SIGNAL(textChanged()), this, SLOT(changeStylesheet()));
    connect(ui->plArtSaveBox,       SIGNAL(toggled(bool)), this, SLOT(changePlArtSave(bool)));
    connect(ui->plArtSaveEdit,      SIGNAL(textChanged(QString)),     this, SLOT(changePlArtFilename(QString)));
    connect(ui->gridBox,            SIGNAL(currentIndexChanged(int)), this, SLOT(changeGridStyle(int)));
    connect(ui->copyPatternEdit,    SIGNAL(textChanged(QString)),     this, SLOT(changeCopyPattern(QString)));



        //! Columns
    connect(ui->colWidth,       SIGNAL(textChanged(QString)),      this,   SLOT(changeColWidth(QString)));
    connect(ui->colHeight,      SIGNAL(textChanged(QString)),      this,   SLOT(changeRowHeight(QString)));
    connect(ui->colAlignBox,    SIGNAL(currentIndexChanged(int)),  this,   SLOT(changeColAlign(int)));
    connect(ui->colTextEdit,    SIGNAL(textChanged(QString)),      this,   SLOT(changeColText(QString)));
    connect(ui->colColorEdit,   SIGNAL(textChanged(QString)),      this,   SLOT(changeColTextColor(QString)));
    connect(ui->colColorButton, SIGNAL(clicked()),                 this,   SLOT(openColColorDialog()));
    connect(ui->colList,        SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,   SLOT(beginChangindColName(QListWidgetItem*)));


        //! Groups
    connect(ui->grpShowHeaderBox,   SIGNAL(toggled(bool)),        this, SLOT(changeGrpHeaderVisible(bool)));
    connect(ui->grpByDirs,          SIGNAL(toggled(bool)),        this, SLOT(changeGrpByDirs(bool)));
    connect(ui->grpDelayEdit,       SIGNAL(textChanged(QString)), this, SLOT(changeGrpDelay(QString)));
    connect(ui->grpHeigthEdit,      SIGNAL(textChanged(QString)), this, SLOT(changeGrpHeight(QString)));
    connect(ui->grpTextEdit,        SIGNAL(textChanged(QString)), this, SLOT(changeGrpText(QString)));
    connect(ui->grpVATextEdit,      SIGNAL(textChanged(QString)), this, SLOT(changeGrpVaText(QString)));
    connect(ui->grpColorBackEdit,   SIGNAL(textChanged(QString)), this, SLOT(changeGrpColorBack(QString)));
    connect(ui->grpColorTextEdit,   SIGNAL(textChanged(QString)), this, SLOT(changeGrpColorText(QString)));
    connect(ui->grpColorBackButton, SIGNAL(clicked()),            this, SLOT(openGrpColorBackDialog()));
    connect(ui->grpColorTextButton, SIGNAL(clicked()),            this, SLOT(openGrpColorTextDialog()));
    connect(ui->grpAlignBox,        SIGNAL(currentIndexChanged(int)), this, SLOT(changeGrpTextAlign(int)));
}


void SimplePlaylistPrefsWidget::changeCopyPattern(QString text)
{
    prefs->copy_preset = text;
    emit copyPatternChanged(text);
}


void SimplePlaylistPrefsWidget::changeColName(QListWidgetItem *, QListWidgetItem *previous)
{
    qDebug() << previous->text();

    if (editedCol > -1)
    {
        disconnect(ui->colList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                   this, SLOT(changeColName(QListWidgetItem*,QListWidgetItem*)));

        if (prefs->columns_names.at(editedCol) != previous->text())
        {
            prefs->columns_names.replace(editedCol, previous->text());
            emit colNameChanged(editedCol, previous->text());
        }

        editedCol = -1;
    }
}


void SimplePlaylistPrefsWidget::beginChangindColName(QListWidgetItem *item)
{
    editedCol = ui->colList->row(item);
    qDebug() << editedCol;

    connect(ui->colList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changeColName(QListWidgetItem*,QListWidgetItem*)));
}


void SimplePlaylistPrefsWidget::createContextMenus()
{
    ui->colList->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *act;

    act = new QAction(tr("Move up"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(moveColUp()));
    ui->colList->addAction(act);

    act = new QAction(tr("Move down"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(moveColDown()));
    ui->colList->addAction(act);


    act = new QAction(this);
    act->setSeparator(true);
    ui->colList->addAction(act);


    act = new QAction(tr("Add new column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(addNewColumn()));
    ui->colList->addAction(act);

    act = new QAction(tr("Remove column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(removeColumn()));
    ui->colList->addAction(act);
}


void SimplePlaylistPrefsWidget::moveColUp()
{
    int col = ui->colList->currentRow();

    if (!col)
        return;

    prefs->columns_aligment.move(col, col - 1);
    prefs->columns_names.move(col, col - 1);
    prefs->columns_sizes.move(col, col - 1);
    prefs->columns_text_color.move(col, col - 1);
    prefs->rows_format.move(col, col - 1);

    fillColNamesList();
    ui->colList->setCurrentRow(col - 1);

    emit colMoved(col, col - 1);
}


void SimplePlaylistPrefsWidget::moveColDown()
{
    int col = ui->colList->currentRow();

    if ( (col >= ui->colList->count() - 1) | !ui->colList->count() )
        return;

    prefs->columns_aligment.move(col, col + 1);
    prefs->columns_names.move(col, col + 1);
    prefs->columns_sizes.move(col, col + 1);
    prefs->columns_text_color.move(col, col + 1);
    prefs->rows_format.move(col, col + 1);

    fillColNamesList();
    ui->colList->setCurrentRow(col + 1);

    emit colMoved(col, col + 1);
}


void SimplePlaylistPrefsWidget::addNewColumn()
{
    int id = ui->colList->currentRow();

    prefs->columns_aligment.insert(id + 1, Qt::AlignLeft);
    prefs->columns_names.insert(id + 1, tr("New column"));
    prefs->columns_sizes.insert(id + 1, 100);
    prefs->columns_text_color.insert(id + 1, QColor());
    prefs->rows_format.insert(id + 1, QString::null);

    fillColNamesList();
    ui->colList->setCurrentRow(id + 1);

    if (ui->colList->count() == 1)
    {
        ui->colPropertiesGroup->setEnabled(true);
        ui->colTextEdit->setEnabled(true);
        ui->lblColText->setEnabled(true);
    }

    emit colInserted(id + 1);
}


void SimplePlaylistPrefsWidget::changeGrpVaText(QString text)
{
    prefs->groups_va_format = text;
    emit grpVaTextChanged(text);
}


void SimplePlaylistPrefsWidget::removeColumn()
{
    if (!ui->colList->count())
        return;

    int id = ui->colList->currentRow();

    prefs->columns_names.removeAt(id);
    prefs->columns_aligment.removeAt(id);
    prefs->columns_sizes.removeAt(id);
    prefs->columns_text_color.removeAt(id);
    prefs->rows_format.removeAt(id);

    fillColNamesList();
    if (ui->colList->count() > id)
        ui->colList->setCurrentRow(id);
    else if (ui->colList->count())
        ui->colList->setCurrentRow(ui->colList->count() - 1);
    else
    {
        ui->colPropertiesGroup->setEnabled(false);
        ui->colTextEdit->setEnabled(false);
        ui->lblColText->setEnabled(false);
    }

    emit colRemoved(id + 1);
}


void SimplePlaylistPrefsWidget::changeGridStyle(int boxIndex)
{
    Qt::PenStyle style = prefs->grid_style;

    switch (boxIndex) {
    case 0: style = Qt::SolidLine;      break;
    case 1: style = Qt::DashLine;       break;
    case 2: style = Qt::DotLine;        break;
    case 3: style = Qt::DashDotLine;    break;
    case 4: style = Qt::DashDotDotLine; break;
    default: qWarning() << "SimplePlaylistPrefsWidget::changeGridStyle()\n\t Undefined index:" << boxIndex; break;
    }

    prefs->grid_style = style;
    emit gridStyleChanged(style);
}


void SimplePlaylistPrefsWidget::changeGridVisible(bool visible)
{
    prefs->show_grid = visible;
    emit gridVisibleChanged(visible);
}


void SimplePlaylistPrefsWidget::openGrpColorTextDialog()
{
    QColor color = QColorDialog::getColor(prefs->groups_text_color, this,
                     tr("Choose color for group-line text"));

    if (!color.isValid() || (color == prefs->groups_text_color))
        return;

    ui->grpColorTextEdit->setText(color.name());
}


void SimplePlaylistPrefsWidget::openGrpColorBackDialog()
{
    QColor color = QColorDialog::getColor(prefs->groups_back_color, this,
                     tr("Choose color for group-line background"));

    if (!color.isValid() || (color == prefs->groups_back_color))
        return;

    ui->grpColorBackEdit->setText(color.name());
}


void SimplePlaylistPrefsWidget::changeGrpColorText(QString text)
{
    QColor color;
    color.setNamedColor(text);
    prefs->groups_text_color = color;
    emit grpColorTextChanged(color);
}


void SimplePlaylistPrefsWidget::changeGrpColorBack(QString text)
{
    QColor color;
    color.setNamedColor(text);
    prefs->groups_back_color = color;
    emit grpColorBackChanged(color);
}


void SimplePlaylistPrefsWidget::changeGrpTextAlign(int boxIndex)
{
    int align = getAlignFromBox(boxIndex);

    if (prefs->groups_text_aligment != align)
    {
        prefs->groups_text_aligment = align;
        emit grpTextAlignChanged(align);
    }
}


void SimplePlaylistPrefsWidget::changeGrpText(QString text)
{
    prefs->groups_format = text;
    emit grpTextChanged(text);
}


void SimplePlaylistPrefsWidget::changeGrpHeight(QString text)
{
    if (text.isEmpty())
        prefs->group_height = 0;
    else
    {
        bool ok;
        int height = text.toInt(&ok);

        if (!ok) return;

        prefs->group_height = height;
        emit grpHeightChanged(height);
    }
}


void SimplePlaylistPrefsWidget::changeGrpDelay(QString text)
{
    if (text.isEmpty())
        prefs->group_delay = 0;
    else
    {
        bool ok;
        int delay = text.toInt(&ok);

        if (!ok) return;

        prefs->group_delay = delay;
        emit grpDelayChanged(delay);
    }
}


void SimplePlaylistPrefsWidget::changeGrpByDirs(bool enable)
{
    prefs->group_byDirs = enable;
    emit grpByDirsChanged(enable);
}


void SimplePlaylistPrefsWidget::changeGrpHeaderVisible(bool visible)
{
    prefs->group_header = visible;
    emit grpHeaderVisibleChanged(visible);
}


void SimplePlaylistPrefsWidget::openColColorDialog()
{
    QColor color = QColorDialog::getColor(prefs->columns_text_color.at(ui->colList->currentRow()),
                     this,
                     tr("Choose color for column") + "\"" + ui->colList->currentItem()->text() + "\"");

    if (!color.isValid() || (color == prefs->columns_text_color.at(ui->colList->currentRow())))
        return;

    ui->colColorEdit->setText(color.name());
}


void SimplePlaylistPrefsWidget::changeColTextColor(QString color)
{
    if (!ui->colList->count())
        return;

    QColor c_color;
    c_color.setNamedColor(color);

    if (c_color != prefs->columns_text_color.at(ui->colList->currentRow()))
    {
        prefs->columns_text_color.replace(ui->colList->currentRow(), c_color);
        emit colTextColorChanged(ui->colList->currentRow(), c_color);
    }
}


void SimplePlaylistPrefsWidget::changeColText(QString text)
{
    if (!ui->colList->count())
        return;

    if (prefs->rows_format.at(ui->colList->currentRow()) == text)
        return;

    prefs->rows_format.replace(ui->colList->currentRow(), text);

    if (text.contains("%art%"))
        setColPropersiesEnabled(false);
    else
        setColPropersiesEnabled(true);

    emit colTextChanged(ui->colList->currentRow(), text);
}


void SimplePlaylistPrefsWidget::setColPropersiesEnabled(bool enable)
{
    ui->colAlignBox->setEnabled(enable);
    ui->colColorButton->setEnabled(enable);
    ui->colColorEdit->setEnabled(enable);
    ui->colHeight->setEnabled(enable);

    ui->lblColAlign->setEnabled(enable);
    ui->lblColColor->setEnabled(enable);
    ui->lblColHeight->setEnabled(enable);
}


void SimplePlaylistPrefsWidget::changeColAlign(int boxIndex)
{
    if (!ui->colList->count())
        return;

    int align = getAlignFromBox(boxIndex);

    if (prefs->columns_aligment.at(ui->colList->currentRow()) != align)
    {
        prefs->columns_aligment.replace(ui->colList->currentRow(), align);
        emit colAlignChanged(ui->colList->currentRow(), align);
    }
}


Qt::AlignmentFlag SimplePlaylistPrefsWidget::getAlignFromBox(int boxIndex)
{
    switch (boxIndex) {
    case 0: return Qt::AlignLeft;
    case 1: return Qt::AlignRight;
    case 2: return Qt::AlignHCenter;
    case 3: return Qt::AlignJustify;
    default: qWarning() << "SimplePlaylistPrefsWidget::getAlignFromBox()\n\t Undefined index:" << boxIndex; break;
    }

    return Qt::AlignLeft;
}


void SimplePlaylistPrefsWidget::changeRowHeight(QString text)
{
    if (prefs->row_height == text.toInt())
        return;

    if (text.isEmpty())
    {
        prefs->row_height = 0;
        emit rowHeightChanged(0);
    }
    else
    {
        bool ok;
        int size = text.toInt(&ok);

        if (!ok) return;

        prefs->row_height = size;
        emit rowHeightChanged(size);
    }
}


void SimplePlaylistPrefsWidget::changeColWidth(QString text)
{
    if (!ui->colList->count())
        return;

    if ( prefs->columns_sizes.at(ui->colList->currentRow()) == text.toInt() )
        return;

    if (text.isEmpty())
        prefs->columns_sizes.replace(ui->colList->currentRow(), 0);
    else
    {
        bool ok;
        int size = text.toInt(&ok);

        if (!ok) return;

        prefs->columns_sizes.replace(ui->colList->currentRow(), size);
        emit colWidthChanged(ui->colList->currentRow(), size);
    }
}


void SimplePlaylistPrefsWidget::changePlArtFilename(QString filename)
{
    Global::pref->pl_art_filename = filename;
}


void SimplePlaylistPrefsWidget::changeStylesheet()
{
    prefs->stylesheet = ui->styleEdit->toPlainText();
    emit stylesheetChanged(prefs->stylesheet);
}


void SimplePlaylistPrefsWidget::changePlArtSave(bool enable)
{
    Global::pref->use_pl_art = enable;
    ui->plArtSaveEdit->setEnabled(enable);
}


void SimplePlaylistPrefsWidget::changeHeaderVisible(bool enable)
{
    prefs->show_header = enable;
    emit headerVisibleChanged(enable);
}


void SimplePlaylistPrefsWidget::fillArtSearchLists()
{
    ui->artFoldersList->clear();
    ui->artFilesList->clear();

    foreach(QString folder, Global::pref->art_search_folders)
        ui->artFoldersList->addItem(new QListWidgetItem(folder));

    foreach(QString file, Global::pref->art_search_patterns)
        ui->artFilesList->addItem(new QListWidgetItem(file));
}


void SimplePlaylistPrefsWidget::fillColNamesList()
{
    ui->colList->clear();

    foreach(QString column, prefs->columns_names)
    {
        QListWidgetItem *item = new QListWidgetItem(column);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->colList->addItem(item);
    }
}


void SimplePlaylistPrefsWidget::columnChosen(int col)
{
    if (col < 0)
        return;

    ui->colWidth->setText(QString::number(prefs->columns_sizes.at(col)));
    ui->colHeight->setText(QString::number(prefs->row_height));
    setAlignBoxState(ui->colAlignBox, prefs->columns_aligment.at(col));
    ui->colTextEdit->setText(prefs->rows_format.at(col));

    if (prefs->columns_text_color.at(col).isValid())
        ui->colColorEdit->setText(prefs->columns_text_color.at(col).name());
    else
        ui->colColorEdit->setText(QString::null);
}


void SimplePlaylistPrefsWidget::setAlignBoxState(QComboBox *box, int alignment)
{
    switch (alignment) {
    case Qt::AlignLeft:    box->setCurrentIndex(0); break;
    case Qt::AlignRight:   box->setCurrentIndex(1); break;
    case Qt::AlignHCenter: box->setCurrentIndex(2); break;
    case Qt::AlignJustify: box->setCurrentIndex(3); break;
    default: qWarning() << "SimplePlaylistPrefsWidget::setAlignBoxState()\n\t Undefined aligment:" << alignment; break;
    }
}


void SimplePlaylistPrefsWidget::setGridStyleBoxState(Qt::PenStyle style)
{
    switch (style) {
    case Qt::SolidLine:      ui->gridBox->setCurrentIndex(0); break;
    case Qt::DashLine:       ui->gridBox->setCurrentIndex(1); break;
    case Qt::DotLine:        ui->gridBox->setCurrentIndex(2); break;
    case Qt::DashDotLine:    ui->gridBox->setCurrentIndex(3); break;
    case Qt::DashDotDotLine: ui->gridBox->setCurrentIndex(4); break;
    default: qWarning() << "SimplePlaylistPrefsWidget::setGridStyleBoxState()\n\t Undefined PenStyle:" << style; break;
    }
}


void SimplePlaylistPrefsWidget::changeAlternateColors(bool enable)
{
    prefs->alternate_colors = enable;
    emit alternateColorsChanged(enable);
}


