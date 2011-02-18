#ifndef QUEUEINTERFACE_H
#define QUEUEINTERFACE_H

#include <QObject>
#include <QList>

class QueueInterface : public QObject
{
    Q_OBJECT
public:
    explicit QueueInterface(QObject *parent = 0);


signals:
    void one_track_left(int guid);
    void queue_is_empty();
    void duration_changed(int sec);
    void track_added(int guid);
    void track_removed(int guid, int id);   //! send 'guid' and 'id'_in_queue
    void track_moved(int idFrom, int idTo);

public slots:
    void enqueue(int guid);
    void enqueue(QList<int> &guids);        //! guids will be emptied!
    int  dequeue();                         //! returns guid, or -1, if list.isEmpty
    int  head();

    int  size();
    bool moveUP(int id);
    bool moveDown(int id);
    bool moveTo(int from, int to);

    bool remove(int id);

    int find(int guid);                  //! returns 'id' in queue
    int get(int id);                     //! returns 'guid'
    QList<int> get(int from, int to);    //! returns empty list, if somethink wrong

    bool isEmpty();

    int duration();

private:
    QList<int> list;

    int lenght;

    bool isInList(int id);

private slots:
    void changeDuration();
    void removed();
};

#endif // QUEUEINTERFACE_H
