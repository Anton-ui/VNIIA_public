#ifndef UDPTAKE_H
#define UDPTAKE_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QGraphicsView>

#include "ik_class.h"

using namespace std;

namespace Ui {
class UDPtake;
}

class UDPtake : public QMainWindow
{
    Q_OBJECT

public:
    explicit UDPtake(QWidget *parent = nullptr);
    ~UDPtake();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::UDPtake *ui;
    unique_ptr<QTimer> timer;
    QGraphicsView*    view_v;
    QGraphicsScene*  scene_v;
    QPixmap         pixmap_v;

    IK_class IK;
    MyThread* thread;
    bool flag = false;

};

#endif // UDPTAKE_H
