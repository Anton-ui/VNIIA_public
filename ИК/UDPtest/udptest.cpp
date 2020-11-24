#include "udptest.h"
#include "ui_udptest.h"

UDPtest::UDPtest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UDPtest)
{
    ui->setupUi(this);

    timer.reset(new QTimer);
    timer->setInterval(1000/5);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    view_v = new QGraphicsView;
    scene_v = new QGraphicsScene(this);
    ui->Layout->addWidget(view_v);
    view_v->setScene(scene_v);
}

UDPtest::~UDPtest()
{
    timer.release();
    delete ui;
}

void UDPtest::on_Start_clicked()
{
    timer->start();
    if (flag == false)
    {
        thread = new MyThread();
        connect(thread, SIGNAL(send()), &IK, SLOT(InitThread()) );
        IK.moveToThread(thread);
        thread->start();
        flag = true;
    }
}

void UDPtest::on_Stop_clicked()
{
    timer->stop();
    IK.stop();
    if (flag == true)
    {
        thread->terminate();
        flag = false;
    }
}

void UDPtest::CaptureImage()
{
    //pixmap_v.loadFromData(IK.GetUcharData(), 0);

    view_v->scene()->clear();
    view_v->scene()->addPixmap(pixmap_v);
    view_v->setScene(scene_v);

    view_v->fitInView(pixmap_v.rect(), Qt::KeepAspectRatio);
}
