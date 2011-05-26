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
#include <QQueue>

using namespace Global;

static QRegExp rx_tag("%([a-z]*)%");


QStringList Helper::getTags(QString pattern)
{
    QStringList list;
    int pos = 0;

    while ((pos = rx_tag.indexIn(pattern, pos)) != -1)
    {
        list.append(rx_tag.cap(1));
        pos += rx_tag.matchedLength();
    }

    return list;
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


QString Helper::valueOfTrack(const QString &tag, QString filePath)
{
    QSqlQuery query(mlib->db);

    if (query.exec("SELECT " + tag + " FROM tracks "
                   "WHERE filepath = '" + filePath.replace("'", "''") + "'"))
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


QString Helper::fileName(const int id)
{
    return valueOfTrack("filepath", id);
}



QString Helper::filePath(const int id)
{
    return valueOfTrack("filedir", id);
}



int Helper::guidOf(QString filename)
{
    if (filename.isEmpty())
        return -1;

    filename.replace("'", "''");


    QSqlQuery query(mlib->db);

    if (query.exec("SELECT id FROM tracks "
                   "WHERE filepath LIKE '" + filename + "'"))
    {
        query.next();
        return query.value(0).toInt();

    }

    qWarning() << query.lastError();
    return -1;
}



QString Helper::processContainer(QString line, int id)
{
    bool emptyFunc = false;

    int size = funcSize(line);
    if (size)
    {
        int lineSize = line.size();
        line = processFunctions(line, id);
        lineSize -= line.size() + 1;

        if (lineSize == size)
            emptyFunc = true;
    }


    QStringList tags   = getTags(line);
    if (tags.isEmpty())
    {
        if (emptyFunc)
            return QString::null;

        line.remove(0, 1);
        line.remove(line.size() - 1, 1);
        return line;
    }

    QStringList values = valueOfTrack(tags, id);


    int ok = 0;

    for (int i = 0; i < values.size(); i++)
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
            int next = nextQuote(line, pos);

            if (next > pos)
            {
                quotes.append(line.mid(pos + 1, next - pos - 1));
                pos = next;
            }
        }
    }

    return quotes;
}


QString Helper::processQuotes(QString line)
{
   foreach(QString quote, getQuotes(line))
        line.replace("'" + quote + "'", quote);

    return line;
}


QString Helper::processTags(QString line, const int id)
{
    QStringList tags = getTags(line);

    if (!tags.isEmpty())
    {
        QStringList values = valueOfTrack(tags, id);

        for (int i = 0; i < values.size(); i++)
            line.replace("%" + tags.at(i) + "%", values.at(i));
    }

    return line;
}


int Helper::nextQuote(const QString &line, int from)
{
    int next = line.indexOf('\'', from+1);
    if (next > -1)
        return next;

    return from;
}


QString Helper::funcIF(QString line, const int id)
{
    QString cond;
    int comma = 0;

    for (int pos = 0; pos < line.size(); pos++)
    {
        QChar chr = line.at(pos);

        if (chr == '\'')
            pos = nextQuote(line, pos);
        else
        if (chr == ',')
            {
            comma = pos;
            cond = line.mid(0, comma);
            break;
        }
    }


    if (!comma)
    {
        qWarning() << QObject::tr("There is no comma in function $if(" + line.toUtf8());
        return QString::null;
    }


    if (isContainer(cond))
        cond = processContainer(cond, id);
    else
        cond = processTags(cond, id);

    if (cond.isEmpty())
    {
        QString tmp = line.mid(comma + 1);

        if (isContainer(tmp))
            return processContainer(tmp, id);

        return processTags(tmp, id);

    } else {
        return cond;
    }

    return QString::null;
}


bool Helper::isContainer(const QString &line)
{
    if (line.isEmpty())
        return false;

    if ((line.at(0) == '[') && (line.at(line.size() - 1) == ']'))
        return true;

    return false;
}


QString Helper::doFunc(const QString &func, QString line, const int id)
{
    if (func == "if")
        return funcIF(line, id);


    qWarning() << QObject::tr("I'm not familiar with function \"$" + func.toUtf8());
    return QString::null;
}


QString Helper::processFunctions(QString line, const int id)
{
    QString func;
    int start = 0;
    int end   = 0;
    int body  = 0;

    for (int pos = 0; pos < line.size(); pos++)
    {
        QChar chr = line.at(pos);

        if (chr == '\'')
            pos = nextQuote(line, pos);
        else
        if (chr == '$')
            start = pos;
        else
        if (chr == '(') {
            body = pos;
            func = line.mid(start + 1, body - start - 1);
        }
        else
        if (chr == ')') {
            end = pos;
            break;
        }
    }


    if (start && body && (!func.isEmpty()) && end)
    {
        func = doFunc(func, line.mid(body + 1, end - body - 1), id);
        line.replace(start, end - start + 1, func);

        return processFunctions(line, id);
    }

    return line;
}


QString Helper::processContainers(QString line, const int id)
{
    QQueue<int> open;
    QQueue<int> closed;

    for (int pos = 0; pos < line.size(); pos++)
    {
        int opa = open.size();
        int cloza = closed.size();

        QChar chr = line.at(pos);

        if (chr == '\'')
            pos = nextQuote(line, pos);

        else
        if (chr == '$')
            pos = funcEnd(line, pos);

        else
        if (chr == '[')
            open.enqueue(pos);

        else
        if (chr == ']')
        {
            closed.enqueue(pos);

            cloza = closed.size();
            opa = open.size();

            if (open.size() == closed.size())
            {
                int opn = open.takeLast();
                int cls = closed.dequeue();

                int i=1;

                while (opn > cls)
                {
                    open.insert(open.size() - i, opn);
                    opn = open.takeAt(open.size() - i++);
                }

                QString temp = processContainer(line.mid(opn, cls - opn + 1), id);
                line.replace(opn, cls - opn + 1, temp);

                return processContainers(line, id);
            }
        }
    }

    return line;
}


int Helper::funcEnd(const QString &line, const int from)
{
    int start = 0;
    int end   = 0;
    int body  = 0;

    for (int pos = from; pos < line.size(); pos++)
    {
        QChar chr = line.at(pos);

        if (chr == '\'')
            pos = nextQuote(line, pos);
        else
        if (chr == '$')
            start = pos;
        else
        if (chr == '(')
            body = pos;
        else
        if (chr == ')') {
            end = pos;
            break;
        }
    }

    if (body && end)
        return end;

    return from;
}

int Helper::funcSize(const QString &line, const int from)
{
    int start = 0;
    int end   = 0;
    int body  = 0;

    for (int pos = from; pos < line.size(); pos++)
    {
        QChar chr = line.at(pos);

        if (chr == '\'')
            pos = nextQuote(line, pos);
        else
        if (chr == '$')
            start = pos;
        else
        if (chr == '(')
            body = pos;
        else
        if (chr == ')') {
            end = pos;
            break;
        }
    }

    if (body && end)
        return end - start;

    return 0;
}


QString Helper::parseLine(const int id, QString line)
{
    line = processContainers(line, id);
    line = processFunctions(line, id);
    line = processTags(line, id);
    line = processQuotes(line);

    return line;
}
