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
    void headerChanged(QString text);
    void patternChanged(QString pattern);
    void iconChanged(QString filename);
    void stylesheetChanged();

private:
    void load();
    void conct();

private:
    Ui::AlbumTreePrefsWidget *ui;
    AlbumTreePrefs           *prefs;

private slots:
    inline void setTreeHeader(QString text)  { prefs->header = text;       emit headerChanged(text); }
    inline void setTreeHeaderVisible(bool b) { prefs->showHeader = b;      emit headerChanged(QString::null); }
    inline void setTreePattern(QString text) { prefs->pattern = text;      emit patternChanged(text); }

    void setTreeItemIcon(QString text);
    void setTreeStyle();

    void getIcon();
};

#endif // ALBUMTREEPREFSWIDGET_H
