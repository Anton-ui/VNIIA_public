#include "cam.h"
#include "ui_cam.h"

QPen pen_red(Qt::red);

Cam::Cam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cam)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    pixmap_vector.resize(number);
    mat_vector.resize(number);

    ui->setupUi(this);
    timer.reset(new QTimer);
    timer->setInterval(1000/30);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    for (int var = 0; var < number; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        cam_vector[var].InitCamera(var, number);
        view_vector[var]->setScene(scene_vector[var]);
    }
}

Cam::~Cam()
{
    delete ui;
    timer.release();
}

void Cam::on_Start_clicked()
{
    timer->start();
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
            cam_vector[var].Start();
        else qDebug() << "Camera" << var << "not started";
}

void Cam::on_Stop_clicked()
{
    for (int var = 0; var < number; ++var)
        cam_vector[var].Stop();
    timer->stop();
}

void Cam::CaptureImage()
{
    for (int var = 0; var < number; var++)
        if(cam_vector[var].Status() == XI_ON)
        {
            mat_vector[var] = cam_vector[var].GetMatImage();

            view_vector[var]->scene()->clear();
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);

            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
        }
}
