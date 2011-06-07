
#include "minfo.h"
#include "global.h"
#include <QFileInfo>
#include <QStringList>
#include <QFile>

using namespace Global;

QString Minfo::findArt(QString path)
{
    QStringList folders;
    folders.append(path);

    foreach (QString str, pref->art_search_folders)
    {
        str = path + "/" + str;
        if (QFile::exists(str))
            folders.append(path + "/" + str);
    }

    if (!pref->music_library_path.contains(path))
    {
        QStringList list(path.split("/"));
        path = list.join("/").remove(list.last());

        folders.append(path);

        foreach (QString str, pref->art_search_folders)
        {
            str = path + "/" + str;
            if (QFile::exists(str))
                folders.append(path + "/" + str);
        }
    }

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



QString Minfo::findPlArt(QDir dir)
{
    if (dir.exists(pref->pl_art_filename))
        return dir.absoluteFilePath(pref->pl_art_filename);

    return QString::null;
}


void Minfo::fixMetadata(QString filepath, QMultiMap<QString, QString> *meta)
{
    QStringList path = filepath.split("/");

    meta->insert("FORMAT", path.last().mid(path.last().lastIndexOf(".") + 1).toUpper());

    if (meta->value("TITLE").isEmpty())
    {
        QString name = path.last().mid(0, path.last().lastIndexOf("."));

        bool ok;
        int i=0;

        QString(name.at(0)).toInt(&ok);
        while (ok)
        {
            i++;
            QString(name.at(i)).toInt(&ok);
        }

        int num = name.mid(0, i).toInt(&ok);

        if (ok)
        {
            name.remove(0, i);

            QChar chr = name.at(0);
            while ((chr == '.') | (chr == '-')
                 | (chr == ' ') | (chr == '_'))
            {
                name.remove(0, 1);
                chr = name.at(0);
            }

            meta->replace("TRACK-NUMBER", QString::number(num));
        }


        i = name.indexOf(" - ");

        if (i > -1)
        {
            meta->replace("ARTIST", name.mid(0, i));
            meta->replace("TITLE", name.mid(i+3));

        } else {
            meta->replace("TITLE", name);
        }


    }


    if (meta->value("TRACK-NUMBER").size() == 1)
        meta->replace("TRACK-NUMBER", "0" + meta->value("TRACK-NUMBER"));

    if (meta->value("ALBUM").isEmpty())
        meta->replace("ALBUM", path.at(path.size() - 2));

}
