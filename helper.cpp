#include "helper.h"
#include "global.h"
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>
#include <QTime>

using namespace Global;

static QRegExp rx_tag("%([a-z]*)%");


QString Helper::parseLine(const int GUID, QString pattern)
{
    QString s = pattern;

    QStringList tags = getTags(pattern);
    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tags.join(", ") + " FROM tracks "
                   "WHERE id = '" + QString::number(GUID) + "'"))
    {
        query.next();
        for (int i = 0; i < tags.size(); i++)
        {
            if (tags.at(i) != "duration")
                s.replace(tags.at(i), query.value(i).toString());
            else
                s.replace(tags.at(i), QTime(0, 0).addMSecs(query.value(i).toInt()).toString());
        }
        s.remove("%");

    } else {
        qWarning() << query.lastError();
    }

    return s;
}


QStringList Helper::getTags(QString pattern)
{
    QStringList list;
    int pos = 0;

    while ((pos = rx_tag.indexIn(pattern, pos)) != -1)
    {
        list.append(rx_tag.cap(1));
        pos += rx_tag.matchedLength();
    }

    return list; // тэги из паттерна одной ветви
}


QString Helper::formatTime(int sec) const
{
    int hours = (int) sec / 3600;
    sec -= hours*3600;
    int minutes = (int) sec / 60;
    sec -= minutes*60;
    int seconds = sec;

    QString tf;
    return tf.sprintf("%02d:%02d:%02d",hours,minutes,seconds);
}


QString Helper::fileName(const int GUID)
{
    QString s;
    QSqlQuery query(mlib->db);

    if (query.exec("SELECT filepath, filename FROM tracks "
                   "WHERE id = '" + QString::number(GUID) + "'"))
    {
        query.next();
        s.append(query.value(0).toString());
        s.append(query.value(1).toString());

    } else {
        qWarning() << query.lastError();
    }

    return s;
}



QString Helper::filePath(const int GUID)
{
    QString s;

    QSqlQuery query(mlib->db);
    if (query.exec("SELECT filepath FROM tracks "
                   "WHERE id = '" + QString::number(GUID) + "'"))
    {
        query.next();
        s = query.value(0).toString();
    }
    return s;
}



int Helper::guidOf(QString filename)
{
    filename.replace("'", "''");

    QStringList path = filename.split(QDir::separator());
    path.removeLast();

    QString f = filename.remove(path.join(QDir::separator()));

    QSqlQuery query(mlib->db);

    if (query.exec("SELECT id FROM tracks "
                   "WHERE (filename LIKE '" + path.join(QDir::separator()) + "') "
                   "AND (filepath LIKE '" + f + "')"))
    {
        query.next();
        return query.value(0).toInt();

    }

    qWarning() << query.lastError();
    return -1;
}
