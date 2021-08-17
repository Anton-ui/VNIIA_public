#ifndef IK_H
#define IK_H

//https://zalinux.ru/?p=1925

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QGraphicsView>

#include "ICamera/pcapinterface.h"

using namespace std;

namespace Ui {
class IK;
}

class IK : public QMainWindow
{
    Q_OBJECT

public:
    explicit IK(QWidget *parent = nullptr);
    ~IK();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::IK *ui;
    unique_ptr<QTimer> timer;
    QGraphicsView*    view_v;
    QGraphicsScene*  scene_v;
    QPixmap         pixmap_v;

    TripleBuf         *buffer;
    pcapDeviceSniffer sniffer;
    uchar *UserData;
};

#endif // IK_H
