#include "thread_buffer.h"

Buffer::Buffer()
{
    for (int i = 0; i < 3; ++i)
    {
        images[i] = nullptr;
    }
}

Buffer::~Buffer()
{
    for (int i = 0; i < 3; ++i)
    {
        images[i] = nullptr;
    }
}

void Buffer::swap()
{
    mutex.lock();
    uchar* swap = images[0];
    images[0] = images[1];
    images[1] = images[2];
    images[2] = swap;
    newFrame = true;
    mutex.unlock();
    qDebug() << "swapped";
}


//#################################

MyThread::MyThread()
{

}

void MyThread::run()
{
    emit send();
}
