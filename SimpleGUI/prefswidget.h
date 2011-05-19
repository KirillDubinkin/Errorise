#ifndef PREFSWIDGET_H
#define PREFSWIDGET_H

#include <QWidget>
#include <QHash>
#include <QListWidget>
#include <QStackedWidget>

class PrefsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrefsWidget(QWidget *parent = 0);

signals:

public slots:
    void addPrefsWidget(QString name, QWidget *widget);
    void show();

private:
    QHash<QString, QWidget *> widgetHash;
    QStackedWidget *stack;
    QListWidget    *listWidget;

};

#endif // PREFSWIDGET_H
