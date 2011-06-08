#include "prefswidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QHashIterator>
#include <QListWidgetItem>

PrefsWidget::PrefsWidget(QList<QWidget *> prefsWidgetList,
                         QWidget *defaultWidget, QWidget *parent) :
    QWidget(parent)
{
    listWidget = 0;
    stack      = 0;
    defWidget  = defaultWidget;
    prefsList  = prefsWidgetList;

    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Errorise preferences"));
    setAttribute(Qt::WA_DeleteOnClose);
}


PrefsWidget::~PrefsWidget()
{
    emit geometryChanged(geometry());
}


void PrefsWidget::show()
{
    if (!listWidget) {
        listWidget = new QListWidget(this);
        stack      = new QStackedWidget(this);

        int row = 0;

        foreach (QWidget *widget, prefsList) {
            listWidget->addItem(new QListWidgetItem(widget->windowTitle()));
            stack->addWidget(widget);

            if (widget == defWidget)
                row = listWidget->count() - 1;
        }

        connect(listWidget, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));

        QHBoxLayout *hl = new QHBoxLayout(this);
        hl->addWidget(listWidget, 22);
        hl->addWidget(stack, 80);
        setLayout(hl);

        listWidget->setCurrentRow(row);
    }

    QWidget::show();
}
