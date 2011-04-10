#ifndef SIMPETOOLBARPREFSWIDGET_H
#define SIMPETOOLBARPREFSWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTimer>

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


signals:
    void needTimer();
    void somethingChanged();

private slots:
    void itemChosen();
    void setToolWidth(QString text);
    inline void setToolHeight(QString text) { if (!text.isEmpty()) { prefs->btnHeight = text.toInt(); emit needTimer(); } }
    void setToolText(QString text);
    void setToolIcon(QString filename);
    void setToolStylesheet();
    void getToolIcon();
    void startTimerNow();

private:
    void load();
    void conct();

    void enableAll(bool enable);
    void enableIcons(bool enable);
    void enableText(bool enable);
    void enableSize(bool enable);

    void clearFields();

private:
    Ui::SimpeToolbarPrefsWidget *ui;
    SimpleToolbarPrefs          *prefs;
    QTimer                      timer;
};

#endif // SIMPETOOLBARPREFSWIDGET_H
