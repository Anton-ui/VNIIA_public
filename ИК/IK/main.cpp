#include "ik.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IK w;
    w.show();

    return a.exec();
}
