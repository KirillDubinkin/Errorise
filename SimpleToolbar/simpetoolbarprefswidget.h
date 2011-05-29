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
    void styleChanged(QString stylesheet);

private slots:
    void itemChosen();
    void setToolWidth(QString text);
    void setToolHeight(QString text);
    void setToolText(QString text);
    void setToolIcon(QString filename);
    void setToolStylesheet();
    void getToolIcon();
    void startTimerNow();

    void removeTool();
    void addSpacing();
    void addButtonPlay() { }
    void addButtonPause() {}
    void addButtonPlayOrPause() {}
    void addButtonStop() {}
    void addButtonNext() {}
    void addButtonPrev() {}
    void addSeekBar();
    void addVolumeBar();
    void moveUp() {}
    void moveDown() {}

private:
    void load();
    void conct();
    void loadToolList();
    void createToolListMenu();

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
