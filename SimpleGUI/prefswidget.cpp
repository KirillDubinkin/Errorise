#include "prefswidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QListWidgetItem>

PrefsWidget::PrefsWidget(QList<QWidget *> prefsWidgetList,
                         QWidget *defaultWidget, QWidget *parent) :
    QMainWindow(parent)
{
    listWidget = 0;
    stack      = 0;
    defWidget  = defaultWidget;
    prefsList  = prefsWidgetList;

    setStatusBar(new QStatusBar(this));

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
        listWidget = new QListWidget();
        stack      = new QStackedWidget();

        int row = 0;

        foreach (QWidget *widget, prefsList) {
            listWidget->addItem(new QListWidgetItem(widget->windowTitle()));
            stack->addWidget(widget);

            if (widget == defWidget)
                row = listWidget->count() - 1;
        }

        connect(listWidget, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));

        QHBoxLayout *hl = new QHBoxLayout();
        hl->addWidget(listWidget, 22);
        hl->addWidget(stack, 80);

        QWidget *w = new QWidget;
        w->setLayout(hl);
        setCentralWidget(w);

        listWidget->setCurrentRow(row);
    }

    QWidget::show();
}
