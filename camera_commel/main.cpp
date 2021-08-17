#include "cam.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cam w;
    w.show();

    return a.exec();
}
