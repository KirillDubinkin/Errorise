#ifndef SIMPLEGUIPREFS_H
#define SIMPLEGUIPREFS_H

#include <QRect>
#include <QString>

class SimpleGUIPrefs
{
public:
    SimpleGUIPrefs();
    ~SimpleGUIPrefs();

    void reset();
    void load();
    void save();

    QRect   geometry;
    QString title_format;

};

#endif // SIMPLEGUIPREFS_H
