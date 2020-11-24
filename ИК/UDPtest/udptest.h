#ifndef UDPTEST_H
#define UDPTEST_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QGraphicsView>

#include "ik_class.h"

using namespace std;

namespace Ui {
class UDPtest;
}

class UDPtest : public QMainWindow
{
    Q_OBJECT

public:
    explicit UDPtest(QWidget *parent = nullptr);
    ~UDPtest();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::UDPtest *ui;
    unique_ptr<QTimer> timer;
    QGraphicsView*    view_v;
    QGraphicsScene*  scene_v;
    QPixmap         pixmap_v;

    IK_class IK;
    MyThread* thread;
    bool flag = false;
};

#endif // UDPTEST_H
