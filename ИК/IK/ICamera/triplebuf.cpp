#include "triplebuf.h"

TripleBuf::TripleBuf()
{
    uchar *ptrTemp;
    for(int i=0;i<3;++i)
    {
        ptrTemp = &ImgBuf[i][0];
        setPages(i,ptrTemp);
    }
    qDebug()<< "triplebuf thread: "<<QThread::currentThreadId();
}

bool TripleBuf::NewFrame()
{
    return newFrame;
}

