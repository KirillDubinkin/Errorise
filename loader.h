#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include <QWidget>
#include <QStringList>
#include <QApplication>

class Loader : public QObject
{
    Q_OBJECT
public:
    enum Action { play = 0, addToPlaylist = 1, addToQueue = 2 };

    explicit Loader(QObject *parent = 0);
    ~Loader();

    QWidget * gui();
    void      start();
    void      processArgs(QStringList args);

private:
    QWidget *gui_p;
    bool loadTranslations();

};

#endif // LOADER_H
