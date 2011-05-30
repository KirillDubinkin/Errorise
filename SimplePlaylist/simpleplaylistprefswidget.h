#ifndef SIMPLEPLAYLISTPREFSWIDGET_H
#define SIMPLEPLAYLISTPREFSWIDGET_H

#include <QTabWidget>
#include "SimplePlaylist/simpleplprefs.h"
#include <QComboBox>

namespace Ui {
    class SimplePlaylistPrefsWidget;
}

class SimplePlaylistPrefsWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SimplePlaylistPrefsWidget(SimplePLPrefs *prefs, QWidget *parent = 0);
    ~SimplePlaylistPrefsWidget();


private slots:
    void columnChosen(int col);

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
