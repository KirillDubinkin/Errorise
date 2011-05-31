#include "prefswidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QHashIterator>
#include <QListWidgetItem>

#include "global.h"

PrefsWidget::PrefsWidget(QWidget *parent) :
    QWidget(parent)
{
    listWidget = 0;
    stack      = 0;

    setWindowFlags(Qt::Window);
    setWindowTitle("Errorise preferences");
    setAttribute(Qt::WA_DeleteOnClose);
}


PrefsWidget::~PrefsWidget()
{
    Global::pref->last_prefs_widget = listWidget->currentRow();
    emit geometryChanged(geometry());
}

void PrefsWidget::addPrefsWidget(QString name, QWidget *widget)
{
    widgetHash.insert(name, widget);
}


void PrefsWidget::show()
{
    if (!listWidget)
    {
        listWidget = new QListWidget(this);
        stack      = new QStackedWidget(this);

        QHBoxLayout *hl = new QHBoxLayout(this);
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

    if (listWidget->count() > Global::pref->last_prefs_widget)
        listWidget->setCurrentRow(Global::pref->last_prefs_widget);

    QWidget::show();
}
