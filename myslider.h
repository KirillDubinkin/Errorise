#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QMouseEvent>

class MySlider : public QSlider
{
    Q_OBJECT

public:
    explicit MySlider(QWidget *parent = 0, Qt::Orientation orientation = Qt::Horizontal)
    {
        this->setParent(parent);
        this->setOrientation(orientation);
    }

protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->button() == Qt::LeftButton)
        {
            if (orientation() == Qt::Vertical)
                setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
            else
                setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

            event->accept();
        }
        QSlider::mousePressEvent(event);
    }
};


#endif // MYSLIDER_H
