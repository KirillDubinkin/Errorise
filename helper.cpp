#include "helper.h"
#include "global.h"
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>
#include <QTime>
#include <QChar>
#include <QObject>

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


QString Helper::getTag(const QString &pattern)
{
    if (rx_tag.indexIn(pattern) != -1)
        return rx_tag.cap(1);

    return QString::null;
}


QString Helper::valueOfTrack(const QString &tag, int id)
{
    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tag + " FROM tracks "
                   "WHERE id = '" + QString::number(id) + "'"))
    {
        query.next();
        return query.value(0).toString();
    }

    qWarning() << "Helper::valueOfTrack" << query.lastError();
    return QString::null;
}


QStringList Helper::valueOfTrack(const QStringList &tags, int id)
{
    QStringList list;
    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tags.join(", ") + " FROM tracks "
                   "WHERE id = '" + QString::number(id) + "'"))
    {
        query.next();

        for (int i = 0; i < tags.size(); i++)
            list.append(query.value(i).toString());
    }
    else
        qWarning() << "Helper::valueOfTrack(QStringList)" << query.lastError();

    return list;
}


QString Helper::formatTime(int sec)
{
    int hours = (int) sec / 3600;
    sec -= hours*3600;
    int minutes = (int) sec / 60;
    sec -= minutes*60;
    int seconds = sec;

    QString tf;
    return tf.sprintf("%02d:%02d:%02d",hours,minutes,seconds);
}


QString Helper::fileName(const int id)
{
    return valueOfTrack(QStringList() << "filepath" << "filename", id).join(QDir::separator());
}



QString Helper::filePath(const int id)
{
    return valueOfTrack("filepath", id);
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


QString Helper::getHexColors(int r, int g, int b)
{
    QString str = "";
    if (r < 16) str = "0";
    str.append(QString::number(r,16));
    if (g < 16) str.append("0");
    str.append(QString().number(g,16));
    if (b < 16) str.append("0");
    str.append(QString().number(b,16));

    return str;
}


QString Helper::processContainer(QString line, int id)
{
    QStringList tags   = getTags(line);
    QStringList values = valueOfTrack(tags, id);


    QStringList quotes = getQuotes(line);
    foreach (QString quote, quotes)
        line.replace("'" + quote + "'", quote);


    int ok = 0;

    for (int i = 0; i < tags.size(); i++)
    {
        if (!QString(values.at(i)).isEmpty())
        {
            line.replace("%" + tags.at(i) + "%", values.at(i));
            ok++;
        } else
            line.remove("%" + tags.at(i) + "%");
    }

    if (ok)
    {
        line.remove(0, 1);
        line.remove(line.size() - 1, 1);
        return line;
    }

    return QString::null;
}


QStringList Helper::getQuotes(const QString &line)
{
    QStringList quotes;

    for (int pos = 0; pos < line.size(); pos++)
    {
        QChar chr = line.at(pos);

        if (chr == '\'')
        {
            int close = line.indexOf("'", pos + 1);

            if (close > -1)
            {
                quotes.append(line.mid(pos + 1, close - pos - 1));
                pos = close;
            }
            else
                qWarning() << QObject::tr("Lonely quote in " + line.toUtf8());
        }
    }

    return quotes;
}
