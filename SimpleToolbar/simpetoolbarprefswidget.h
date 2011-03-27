#ifndef SIMPETOOLBARPREFSWIDGET_H
#define SIMPETOOLBARPREFSWIDGET_H

#include <QWidget>

namespace Ui {
    class SimpeToolbarPrefsWidget;
}

class SimpeToolbarPrefsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimpeToolbarPrefsWidget(QWidget *parent = 0);
    ~SimpeToolbarPrefsWidget();

private:
    Ui::SimpeToolbarPrefsWidget *ui;
};

#endif // SIMPETOOLBARPREFSWIDGET_H
