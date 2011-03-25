#ifndef ALBUMTREEPREFS_H
#define ALBUMTREEPREFS_H

#include <QString>

class AlbumTreePrefs
{
public:
    AlbumTreePrefs();
    ~AlbumTreePrefs();

    void reset();
    void load();
    void save();

    QString header;
    bool    showHeader;

    QString pattern;
    QString stylesheet;
    QString items_icon;

private:
    QString filename;
};

#endif // ALBUMTREEPREFS_H
