#ifndef TMODERATOR_H
#define TMODERATOR_H

#include <QDebug>

#include <QMutex>
#include "defines.h"

typedef uchar m_type;
enum dataNames
{
    input,
    output,
    buffer
};

class Tmoderator
{
    const static int numberImage = BUFFERS_PAGES;
    m_type* images[numberImage];

    QMutex mutex;

protected:
    Tmoderator();
    void setPages(int i, m_type *p);
    ~Tmoderator();

public:

    bool newFrame;
    m_type *getPtrForInput()  const {return images[input];}
    void changeInput();

    m_type *getPtrForOutput() const {return images[output];}
    bool changeOutput();

};

#endif // TMODERATOR_H
