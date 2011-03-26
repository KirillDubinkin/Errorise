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

private:
    void load();
    void conct();

private:
    Ui::AlbumTreePrefsWidget *ui;
    AlbumTreePrefs           *prefs;

private slots:
    inline void setTreePattern(QString text) { prefs->pattern = text;      emit patternChanged(text); }
    inline void emitStylesheetChange()       { emit stylesheetChanged(prefs->stylesheet); }

    void setTreeItemIcon(QString text);
    void setTreeStyle();

    void getIcon();
};

#endif // ALBUMTREEPREFSWIDGET_H
