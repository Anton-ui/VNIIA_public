#include "ik.h"
#include "ui_ik.h"

IK::IK(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IK)
{
    ui->setupUi(this);

    timer.reset(new QTimer);
    timer->setInterval(1000/25);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    view_v = new QGraphicsView;
    scene_v = new QGraphicsScene(this);
    ui->Layout->addWidget(view_v);
    view_v->setScene(scene_v);

    pcapDeviceSniffer sniffer("169.254.0.13","60:44:48:160:231:123","169.254.0.2","", buffer);
    //3C-2C-30-A0-E7-7B
}

IK::~IK()
{
    timer.release();
    //sniffer.stop();
    delete ui;
}

void IK::on_Start_clicked()
{
    //timer->start();

    sniffer.initInThread();
    //sniffer.CreatePacket(PCKT_TYPE_UDP, UserData, 1286);
}

void IK::on_Stop_clicked()
{
    timer->stop();
}

void IK::CaptureImage()
{
    if (sniffer.trBuffer->NewFrame() == true)
    {
        sniffer.trBuffer->changeOutput();
        pixmap_v.loadFromData( sniffer.trBuffer->getPtrForOutput(), 0 );

        view_v->scene()->clear();
        view_v->scene()->addPixmap(pixmap_v);
        view_v->setScene(scene_v);

        view_v->fitInView(pixmap_v.rect(), Qt::KeepAspectRatio);
    }
}
