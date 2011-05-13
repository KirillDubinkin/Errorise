#ifndef ALBUMTREEPREFSWIDGET_H
#define ALBUMTREEPREFSWIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include "MediaLibrary/albumtreeprefs.h"

namespace Ui {
    class AlbumTreePrefsWidget;
}

class AlbumTreePrefsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumTreePrefsWidget(AlbumTreePrefs *p, QWidget *parent = 0);
    ~AlbumTreePrefsWidget();
    QTimer                   timer;

signals:
    void patternChanged(QString pattern);
    void iconChanged(QString filename);
    void stylesheetChanged(QString style);
    void musicLibraryChanged();

private:
    void load();
    void conct();

private:
    Ui::AlbumTreePrefsWidget *ui;
    AlbumTreePrefs           *prefs;

    QString musicPath;
    QString fileTypes;

private slots:
    inline void setTreePattern(QString text) { prefs->pattern = text;      emit patternChanged(text); }
    inline void emitStylesheetChange()       { emit stylesheetChanged(prefs->stylesheet); }
    void setMusicPath(QString text);
    inline void setFileTypes(QString text)   { fileTypes = text; }

    void setTreeItemIcon(QString text);
    void setTreeStyle();

    void getIcon();
    void getMusicPath();
};

#endif // ALBUMTREEPREFSWIDGET_H
