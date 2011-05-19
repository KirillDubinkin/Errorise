#include "prefswidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QHashIterator>
#include <QListWidgetItem>

PrefsWidget::PrefsWidget(QWidget *parent) :
    QWidget(parent)
{
    listWidget = 0;
    stack      = 0;

    setWindowFlags(Qt::Window);
    setWindowTitle("Errorise preferences");
    setAttribute(Qt::WA_DeleteOnClose);
}


void PrefsWidget::addPrefsWidget(QString name, QWidget *widget)
{
    widgetHash.insert(name, widget);
}


void PrefsWidget::show()
{
    if (!listWidget)
    {
        listWidget = new QListWidget;
        stack      = new QStackedWidget;

        QHBoxLayout *hl = new QHBoxLayout();
        hl->addWidget(listWidget, 20);
        hl->addWidget(stack, 80);
        setLayout(hl);

        QHashIterator<QString, QWidget *> i(widgetHash);
        while (i.hasNext())
        {
            i.next();
            listWidget->addItem(new QListWidgetItem(i.key()));
            stack->addWidget(i.value());
        }

        connect(listWidget, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));

        listWidget->setCurrentRow(0);
    }


    QWidget::show();
}
