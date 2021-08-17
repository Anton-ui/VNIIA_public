#ifndef THREAD_BUFFER_H
#define THREAD_BUFFER_H

#include <QThread>
#include <QMutex>
#include <QDebug>

class Buffer
{
public:
    Buffer();
    ~Buffer();
    uchar *getInputData()  const {return images[0];}
    uchar *getOutputData() const {return images[2];}
    void swap();
    bool newFrame = false;

private:
    QMutex mutex;
    uchar* images[3];
};

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
