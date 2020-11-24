#include "thread_buffer.h"

//#################################

MyThread::MyThread()
{

}

void MyThread::run()
{
    emit send();
}
