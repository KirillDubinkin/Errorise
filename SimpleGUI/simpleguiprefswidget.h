#ifndef SIMPLEGUIPREFSWIDGET_H
#define SIMPLEGUIPREFSWIDGET_H

#include <QWidget>
#include "SimpleGUI/simpleguiprefs.h"

namespace Ui {
    class SimpleGuiPrefsWidget;
}

class SimpleGuiPrefsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleGuiPrefsWidget(SimpleGUIPrefs *p, QWidget *parent = 0);
    ~SimpleGuiPrefsWidget();

signals:
    void windowTitleChanged(QString title);

private slots:
    void changeWindowTitle(QString title)  { prefs->title_format = title;  emit windowTitleChanged(title); }
    void changeLanguage(int boxIndex);

private:
    Ui::SimpleGuiPrefsWidget *ui;
    SimpleGUIPrefs           *prefs;

    void load();
    void conct();

    void fillLanguageBox();
};

#endif // SIMPLEGUIPREFSWIDGET_H
