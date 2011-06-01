#ifndef SIMPLEPLAYLISTPREFSWIDGET_H
#define SIMPLEPLAYLISTPREFSWIDGET_H

#include <QTabWidget>
#include "SimplePlaylist/simpleplprefs.h"
#include <QComboBox>
#include <QString>

namespace Ui {
    class SimplePlaylistPrefsWidget;
}

class SimplePlaylistPrefsWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SimplePlaylistPrefsWidget(SimplePLPrefs *prefs, QWidget *parent = 0);
    ~SimplePlaylistPrefsWidget();


signals:
        //! General
    void alternateColorsChanged(bool enable);
    void headerVisibleChanged(bool enable);
    void stylesheetChanged(QString style);
    void gridVisibleChanged(bool visible);
    void gridStyleChanged(Qt::PenStyle style);

        //! Columns
    void colWidthChanged(int col, int width);
    void colAlignChanged(int col, int align);
    void colTextChanged(int col, QString text);
    void colTextColorChanged(int col, QColor color);
    void rowHeightChanged(int height);

        //! Groups
    void grpHeaderVisibleChanged(bool visible);
    void grpByDirsChanged(bool enable);
    void grpDelayChanged(int delay);
    void grpHeightChanged(int height);
    void grpTextChanged(QString text);
    void grpVaTextChanged(QString text);
    void grpTextAlignChanged(int align);
    void grpColorBackChanged(QColor color);
    void grpColorTextChanged(QColor color);
//! ===============================================================


private slots:
    void columnChosen(int col);

        //! General
    void changeAlternateColors(bool enable);
    void changeHeaderVisible(bool enable);
    void changeStylesheet();
    void changePlArtSave(bool enable);
    void changePlArtFilename(QString filename);
    void changeGridVisible(bool visible);
    void changeGridStyle(int boxIndex);

        //! Columns
    void changeColWidth(QString text);
    void changeRowHeight(QString text);
    void changeColAlign(int boxIndex);
    void changeColText(QString text);
    void changeColTextColor(QString color);
    void openColColorDialog();
    void addNewColumn() {}
    void removeColumn();

        //! Groups
    void changeGrpHeaderVisible(bool visible);
    void changeGrpByDirs(bool enable);
    void changeGrpDelay(QString text);
    void changeGrpHeight(QString text);
    void changeGrpText(QString text);
    void changeGrpVaText(QString text);
    void changeGrpTextAlign(int boxIndex);
    void changeGrpColorText(QString text);
    void changeGrpColorBack(QString text);
    void openGrpColorTextDialog();
    void openGrpColorBackDialog();
//! =================================================================


private:
    Ui::SimplePlaylistPrefsWidget *ui;
    SimplePLPrefs                 *prefs;

    void load();
    void conct();
    void createContextMenus();
    void fillArtSearchLists();
    void fillColNamesList();
    void setColPropersiesEnabled(bool enable);
    void setAlignBoxState(QComboBox *box, int alignment);
    void setGridStyleBoxState(Qt::PenStyle style);
    Qt::AlignmentFlag getAlignFromBox(int boxIndex);
};

#endif // SIMPLEPLAYLISTPREFSWIDGET_H
