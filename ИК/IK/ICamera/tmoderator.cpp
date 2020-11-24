#include "tmoderator.h"

Tmoderator::Tmoderator()
{
    for (int i = 0; i < numberImage; ++i)
    {
        images[i] = NULL;
    }
    newFrame = false;
}

void Tmoderator::setPages(int i, m_type *p)
{
    if (i >= 0 && i < numberImage)
        images[i] = p;
}

Tmoderator::~Tmoderator()
{
    for (int i = 0; i < numberImage; ++i)
    {
        images[i] = NULL;
    }
}

void Tmoderator::changeInput()
{
    mutex.lock();
    m_type* temp = images[input];
    images[input] = images[buffer];
    images[buffer] = temp;
    newFrame = true;
    mutex.unlock();
    qDebug() << "Input changed";
}

bool Tmoderator::changeOutput()
{
    mutex.lock();
    bool res = newFrame;
    if (res)
    {
        m_type* temp = images[buffer];
        images[buffer] = images[output];
        images[output] = temp;
        newFrame = false;
    }
    mutex.unlock();
    return res;
}
