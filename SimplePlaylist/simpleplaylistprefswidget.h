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

private slots:
    void columnChosen(int col);

        //! General
    void changeAlternateColors(bool enable);
    void changeHeaderVisible(bool enable);
    void changeStylesheet();
    void changePlArtSave(bool enable);
    void changePlArtFilename(QString filename);

private:
    Ui::SimplePlaylistPrefsWidget *ui;
    SimplePLPrefs                 *prefs;

    void load();
    void conct();
    void fillArtSearchLists();
    void fillColNamesList();
    void setAlignBoxState(QComboBox *box, int alignment);
};

#endif // SIMPLEPLAYLISTPREFSWIDGET_H
