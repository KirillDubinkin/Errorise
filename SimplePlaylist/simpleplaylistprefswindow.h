#ifndef SIMPLEPLAYLISTPREFSWINDOW_H
#define SIMPLEPLAYLISTPREFSWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QTextEdit>
#include "SimplePlaylist/simpleplprefs.h"

class SimplePlaylistPrefsWindow : public QTabWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylistPrefsWindow(SimplePLPrefs *preferences, QWidget *parent = 0);

signals:

public slots:

private:
    SimplePLPrefs *prefs;

    void createGeneral();
    QCheckBox   *showHeader;
    QCheckBox   *alternateColors;
    QListWidget *artPatterns;
    QListWidget *artFolders;
    QTextEdit   *styleEdit;



};

#endif // SIMPLEPLAYLISTPREFSWINDOW_H
