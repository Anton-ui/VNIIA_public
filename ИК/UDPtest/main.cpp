#include "udptest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UDPtest w;
    w.show();

    return a.exec();
}
