#include "parser.h"
#include <QRegExp>
#include <QDebug>

Parser::Parser(QObject *parent) :
    QObject(parent)
{
}

static QRegExp rx_square("\[.*\]");
static QRegExp rx_sq_open("\[");
static QRegExp rx_sq_closed("\]");
static QRegExp rx_var("%[a-z]*.[a-z]*%");
static QRegExp rx_codec("codec");
static QRegExp rx_bitrate("bitrate");
static QRegExp rx_samplerate("samplerate");
static QRegExp rx_bits("bits");
static QRegExp rx_channels("channels");
static QRegExp rx_playback_time("playback_time");
static QRegExp rx_length("length");


QString Parser::parse(const QString &line)
{
    if (!line.isEmpty())
    {
        qDebug() << line;
        int idx = 0, in, out;
        QString output = line;
//        while (output.length() > idx)
        {
            if ( (in = output.indexOf("[", idx) > -1) & (out = output.indexOf("]", idx) > -1) ) {
                parseSquare(output.mid(in, 5));
            }


        }
    }
    else
    {
        return "";
    }
}

QString Parser::parseSquare(const QString &line)
{
    qDebug() << line;
    if (!line.isEmpty()){
        if (rx_var.indexIn(line) > -1){
            parseVar(line);
        }
    }
}

QString Parser::parseVar(const QString &line)
{
    qDebug() << line;
}

