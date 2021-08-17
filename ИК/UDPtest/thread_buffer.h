#ifndef THREAD_BUFFER_H
#define THREAD_BUFFER_H

#include <QThread>
#include <QMutex>
#include <QDebug>

//###################

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread();
    void run();

signals:
    void send();
};

#endif // THREAD_BUFFER_H
