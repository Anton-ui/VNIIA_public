#include "graph_screen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    graph_screen w;
    w.show();
    return a.exec();
}
