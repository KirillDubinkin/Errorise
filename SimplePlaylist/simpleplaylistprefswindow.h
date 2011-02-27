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
#include <QListWidgetItem>

class SimplePlaylistPrefsWindow : public QTabWidget
{
    Q_OBJECT
public:
    explicit SimplePlaylistPrefsWindow(SimplePLPrefs *preferences, QWidget *parent = 0);

signals:
    void showHeaderChanged(bool);
    void useAlternateColorsChanged(bool);
    void styleEdited(const QString);
    void artSearchChanged();

private slots:
    void setShowHeader(bool show);
    void setAlternateColors(bool b);
    void setPlStyleheet();
    void artFoldersChange(QListWidgetItem *item);
    void artPatternsChange(QListWidgetItem *item);

private:
    SimplePLPrefs *prefs;
    QFont titleFont;


    void createGeneral();
    QCheckBox   *showHeader;
    QCheckBox   *alternateColors;
    QListWidget *artPatterns;
    QListWidget *artFolders;
    QTextEdit   *styleEdit;
    void fillList(QListWidget *list, const QStringList &strlist,
                  Qt::ItemFlags flags);


};

#endif // SIMPLEPLAYLISTPREFSWINDOW_H
