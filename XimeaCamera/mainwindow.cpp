#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

CameraInterface::CameraInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView_2->setScene(new QGraphicsScene(this));

    ui->graphicsView->scene()->addItem(&pixmap_1);
    ui->graphicsView_2->scene()->addItem(&pixmap_2);

    cam_num = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(cam_num);

    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
        cam_vector[var].InitCamera(var, cam_num);
    }
}

CameraInterface::~CameraInterface()
{
    main_timer.release();
    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].Close();
    }
}

void CameraInterface::on_StopPushButton_clicked()
{
    main_timer->stop();
    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].Stop();
    }
}

void CameraInterface::CaptureImage()
{
    if(cam_vector[0].Status() == XI_ON && cam_vector[1].Status()  == XI_ON)
    {
        QPixmap buf_1;
        buf_1.loadFromData(cam_vector[0].GetUcharImage());
        QPixmap buf_2;
        buf_2.loadFromData(cam_vector[1].GetUcharImage());
        pixmap_1.setPixmap(buf_1);
        pixmap_2.setPixmap(buf_2);

        ui->graphicsView->fitInView(&pixmap_1, Qt::KeepAspectRatio);
        ui->graphicsView_2->fitInView(&pixmap_2, Qt::KeepAspectRatio);

        ui->graphicsView->scene()->setSceneRect(pixmap_1.pixmap().rect());
        ui->graphicsView_2->scene()->setSceneRect(pixmap_2.pixmap().rect());
    }
}



void CameraInterface::on_StartPushButton_clicked()
{
    if(cam_vector[0].Status() != XI_ON && cam_vector[1].Status()  != XI_ON)
    {
        for (unsigned long var = 0; var < cam_vector.size(); ++var)
        {
           cam_vector[var].Start();
        }
    }
    main_timer.reset(new QTimer);
    main_timer->setInterval(1000/60);
    connect(main_timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));
    main_timer->start();
}


void CameraInterface::on_GainHorizontalSlider_valueChanged(int value)
{
    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].ChangeGain(value);
    }
}

void CameraInterface::on_ExposureHorizontalSlider_valueChanged(int value)
{

    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].ChangeExposure(value);
    }
}

void CameraInterface::on_decodingSlider_valueChanged(int value)
{
    for (unsigned long var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].ChangeCompression(value);
    }
}

