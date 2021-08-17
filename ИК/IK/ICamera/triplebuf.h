#ifndef TRIPLEBUF_H
#define TRIPLEBUF_H

#include <QObject>
#include "tmoderator.h"
#include "defines.h"

#include <QThread>
#include <QDebug>


typedef uchar WORK_POINT_TYPE;
class TripleBuf : public Tmoderator
{
public:
    TripleBuf();
    WORK_POINT_TYPE ImgBuf[BUFFERS_PAGES][IMG_SIZE*2 + TEMP_INPUT_SIZE*2];
    bool NewFrame(void);

//    Tmoderator<uchar> *Moderator;
};

#endif // TRIPLEBUF_H
