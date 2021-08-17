#include "udptake.h"
#include "ui_udptake.h"

UDPtake::UDPtake(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UDPtake)
{
    ui->setupUi(this);

    timer.reset(new QTimer);
    timer->setInterval(1000/25);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    view_v = new QGraphicsView;
    scene_v = new QGraphicsScene(this);
    ui->Layout->addWidget(view_v);
    view_v->setScene(scene_v);
}

UDPtake::~UDPtake()
{
    timer.release();
    delete ui;
}

void UDPtake::on_Start_clicked()
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

void UDPtake::on_Stop_clicked()
{
    timer->stop();
    IK.stop();
    if (flag == true)
    {
        thread->terminate();
        flag = false;
    }
}

void UDPtake::CaptureImage()
{
    //pixmap_v.loadFromData(IK.GetUcharData(), 0);

    view_v->scene()->clear();
    view_v->scene()->addPixmap(pixmap_v);
    view_v->setScene(scene_v);

    view_v->fitInView(pixmap_v.rect(), Qt::KeepAspectRatio);
}
