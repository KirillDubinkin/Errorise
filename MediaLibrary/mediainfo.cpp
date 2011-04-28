
#include "mediainfo.h"
#include "global.h"
#include <QDir>
#include <QFileInfo>
#include <QStringList>

using namespace Global;

QString MediaInfo::findArt(QString path)
{
    QStringList folders;
    folders.append(path);

    foreach (QString str, pref->art_search_folders)
        folders.append(path + "/" + str);

    QStringList list(path.split("/"));
    path = list.join("/").remove(list.last());

    folders.append(path);

    foreach (QString str, pref->art_search_folders)
        folders.append(path + "/" + str);

    QStringList files;

    for (int i = 0; i < folders.size(); i++)
    {
        QDir dir(folders.at(i));

        for (int j = 0; j < pref->art_search_patterns.size(); j++)
        {
            files = dir.entryList(QStringList(pref->art_search_patterns.at(j)), QDir::Files);
            if (!files.isEmpty())
                return dir.absoluteFilePath(files.first());
        }
    }

    return QString::null;
}
