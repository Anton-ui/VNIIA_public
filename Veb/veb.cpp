#include "veb.h"
#include "ui_veb.h"

Veb::Veb(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Veb),
    cap(0)
{
    ui->setupUi(this);
    timer.reset(new QTimer);
    scene.reset(new QGraphicsScene);
    ui->view->setScene(scene.get());

    timer->setInterval(1000/30);
    connect(timer.get(), SIGNAL(timeout()),this, SLOT(SlotCapture()) );

}

Veb::~Veb()
{
    delete ui;
    timer.release();
    scene.release();
}

void Veb::on_Start_clicked()
{
    timer->start();
}

void Veb::on_Stop_clicked()
{
    timer->stop();
}

void Veb::SlotCapture()
{
    cap >> frame;
    if (value == 0)
        pix = cvMatToQPixmap(frame);
    else {
        GaussianBlur(frame, frame, Size(7, 7), value);
        pix = cvMatToQPixmap(frame);
    }
    scene->clear();
    scene->addPixmap(pix);
    ui->view->setScene(scene.get());
    ui->view->fitInView(pix.rect(), Qt::KeepAspectRatio);
}

void Veb::on_horizontalSlider_valueChanged(int value_new)
{
    value = value_new;
}
