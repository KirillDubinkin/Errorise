#ifndef PREFSWIDGET_H
#define PREFSWIDGET_H

#include <QWidget>
#include <QList>
#include <QListWidget>
#include <QStackedWidget>
#include <QRect>
#include <QMainWindow>

class PrefsWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit PrefsWidget(QList<QWidget *> prefsWidgetList,
                         QWidget *defaultWidget = 0, QWidget *parent = 0);
    virtual ~PrefsWidget();

signals:
    void geometryChanged(QRect geometry);

public slots:
    void show();

private:
    QList<QWidget *> prefsList;
    QWidget        *defWidget;
    QStackedWidget *stack;
    QListWidget    *listWidget;

};

#endif // PREFSWIDGET_H
