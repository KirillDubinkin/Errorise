#ifndef PARSER_H
#define PARSER_H

#include <QObject>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    QString parse(const QString &line);
    QString parseSquare(const QString &line);
    QString parseVar(const QString &line);


signals:

public slots:

};

#endif // PARSER_H
