#ifndef SIMPLEPLAYLISTPREFSWIDGET_H
#define SIMPLEPLAYLISTPREFSWIDGET_H

#include <QTabWidget>

namespace Ui {
    class SimplePlaylistPrefsWidget;
}

class SimplePlaylistPrefsWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SimplePlaylistPrefsWidget(QWidget *parent = 0);
    ~SimplePlaylistPrefsWidget();

private:
    Ui::SimplePlaylistPrefsWidget *ui;
};

#endif // SIMPLEPLAYLISTPREFSWIDGET_H
