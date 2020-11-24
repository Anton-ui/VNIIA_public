#include "udptake.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDPtake w;
    w.show();

    return a.exec();
}
