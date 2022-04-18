#include "graph_windows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    graph_windows w;
    w.show();
    return a.exec();
}
