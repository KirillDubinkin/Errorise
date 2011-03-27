#ifndef SIMPETOOLBARPREFSWIDGET_H
#define SIMPETOOLBARPREFSWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

#include "SimpleToolbar/simpletoolbarprefs.h"

namespace Ui {
    class SimpeToolbarPrefsWidget;
}

class SimpeToolbarPrefsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpeToolbarPrefsWidget(SimpleToolbarPrefs *p, QWidget *parent = 0);
    ~SimpeToolbarPrefsWidget();


private slots:
    void itemChosen(QListWidgetItem *item);

private:
    void load();

    void enableAll(bool enable);
    void enableIcons(bool enable);
    void enableText(bool enable);
    void enableSize(bool enable);

    void clearFields();

private:
    Ui::SimpeToolbarPrefsWidget *ui;
    SimpleToolbarPrefs          *prefs;
};

#endif // SIMPETOOLBARPREFSWIDGET_H
