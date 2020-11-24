#include "veb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Veb w;
    w.show();

    return a.exec();
}
