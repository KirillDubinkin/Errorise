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
    void alternateColorsChanged(bool enable);
    void headerVisibleChanged(bool enable);
    void stylesheetChanged(QString style);
    void colWidthChanged(int col, int width);
    void colAlignChanged(int col, int align);
    void colTextChanged(int col, QString text);
    void colTextColorChanged(int col, QColor color);
    void rowHeightChanged(int height);


private slots:
    void columnChosen(int col);

        //! General
    void changeAlternateColors(bool enable);
    void changeHeaderVisible(bool enable);
    void changeStylesheet();
    void changePlArtSave(bool enable);
    void changePlArtFilename(QString filename);

        //! Columns
    void changeColWidth(QString text);
    void changeRowHeight(QString text);
    void changeColAlign(int boxIndex);
    void changeColText(QString text);
    void changeColTextColor(QString color);
    void openColColorDialog();


private:
    Ui::SimplePlaylistPrefsWidget *ui;
    SimplePLPrefs                 *prefs;

    void load();
    void conct();
    void fillArtSearchLists();
    void fillColNamesList();
    void setAlignBoxState(QComboBox *box, int alignment);
    Qt::AlignmentFlag getAlignFromBox(int boxIndex);
};

#endif // SIMPLEPLAYLISTPREFSWIDGET_H
