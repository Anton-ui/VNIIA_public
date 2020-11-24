#include "cudasurf.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CudaSurf w;
    w.show();

    return a.exec();
}
