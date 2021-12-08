#include "cam.h"
#include "ui_cam.h"

// Запускать это говно только для 1 камеры!

cam::cam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cam)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    pixmap_vector.resize(number);
    mat_vector.resize(number);
    switch_vector.resize(3);

    tick_vector.resize(10);
    count_vector.resize(4);
    sec_vector.resize(4);
    data_vector.resize(5);
    for (int var = 0; var < 4; ++var)
    {
        switch_vector[var] = 0;
        count_vector[var] = 0;
        sec_vector[var] = 0;
        data_vector[var] = 0;
    }
    data_vector[4] = 0;

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

cam::~cam()
{
    timer.release();
    delete ui;
}

void cam::on_Start_clicked()
{
    timer->start();
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
            cam_vector[var].Start();
        else qDebug() << "Camera" << var << "not started";
}

void cam::on_Stop_clicked()
{
    for (int var = 0; var < number; ++var)
        cam_vector[var].Stop();
    timer->stop();
}

void cam::CaptureImage()
{ 
    for (int var = 0; var < number; var++)
        if(cam_vector[var].Status() == XI_ON)
        {
            mat_vector[var] = cam_vector[var].GetMatImage();
            view_vector[var]->scene()->clear();

            if (switch_vector[0])
            {
                tick_vector[0] = getTickCount();
                orb.Method_orb_ON(mat_vector[var], mat_vector[var]);
                tick_vector[1] = getTickCount();
                double secs = (tick_vector[1] - tick_vector[0]) / getTickFrequency();
                if(count_vector[0] < 100)
                {
                    sec_vector[0] += secs;
                    count_vector[0]++;
                }
                else
                {
                    data_vector[0] = sec_vector[0] / 100;
                    qDebug() << "ORB: t =" << data_vector[0];
                    count_vector[0] = 0;
                    sec_vector[0] = 0;
                }
            }
            if (switch_vector[1])
            {
                tick_vector[2] = getTickCount();
                surf.Method_surf_ON(mat_vector[var], mat_vector[var]);
                tick_vector[3] = getTickCount();
                double secs = (tick_vector[3] - tick_vector[2]) / getTickFrequency();
                if(count_vector[1] < 100)
                {
                    sec_vector[1] += secs;
                    count_vector[1]++;
                }
                else
                {
                    data_vector[1] = sec_vector[1] / 100;
                    qDebug() << "SURF: t =" << data_vector[1];
                    count_vector[1] = 0;
                    sec_vector[1] = 0;
                }
            }
            if (switch_vector[2])
            {
                tick_vector[4] = getTickCount();
                akaze.Method_akaze_ON(mat_vector[var], mat_vector[var]);
                tick_vector[5] = getTickCount();
                double secs = (tick_vector[5] - tick_vector[4])/getTickFrequency();
                if(count_vector[2] < 100)
                {
                    sec_vector[2] += secs;
                    count_vector[2]++;
                }
                else
                {
                    data_vector[2] = sec_vector[2] / 100;
                    qDebug() << "AKAZE: t =" << data_vector[2];
                    count_vector[2] = 0;
                    sec_vector[2] = 0;
                }
            }

            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);
            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);     
        }
}

void cam::on_check_orb_stateChanged(int switch1)
{
    if (switch1 != 0)
        switch_vector[0] = true;
    else switch_vector[0] = false;
}

void cam::on_check_surf_stateChanged(int switch2)
{
    if (switch2 != 0)
        switch_vector[1] = true;
    else switch_vector[1] = false;
}

void cam::on_check_akaze_stateChanged(int switch3)
{
    if (switch3 != 0)
        switch_vector[2] = true;
    else switch_vector[2] = false;
}

QPixmap cam::Mat2Pixmap(Mat data)
{
    cvtColor(data, data, COLOR_RGBA2BGRA);
    return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
}
