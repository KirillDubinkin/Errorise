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
#include <QFont>

class SimplePlaylistPrefsWindow : public QTabWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylistPrefsWindow(SimplePLPrefs *preferences, QWidget *parent = 0);

signals:
    void showHeaderChanged(bool);
    void useAlternateColorsChanged(bool);
    void styleEdited(const QString);

private slots:
    void setShowHeader(bool show);
    void setAlternateColors(bool b);
    void setPlStyleheet();

private:
    SimplePLPrefs *prefs;
    QFont titleFont;


    void createGeneral();
    QCheckBox   *showHeader;
    QCheckBox   *alternateColors;
    QListWidget *artPatterns;
    QListWidget *artFolders;
    QTextEdit   *styleEdit;


};

#endif // SIMPLEPLAYLISTPREFSWINDOW_H
