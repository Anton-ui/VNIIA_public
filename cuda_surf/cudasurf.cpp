#include "cudasurf.h"
#include "ui_cudasurf.h"

CudaSurf::CudaSurf(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CudaSurf)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    mat_vector.resize(number);
    grey_mat_vector.resize(number);
    gpumat_vector.resize(number);
    pixmap_vector.resize(number);

    tick_vector.resize(2);
    count_vector.resize(1);
    sec_vector.resize(1);
    data_vector.resize(1);

    count_vector[0] = 0;
    sec_vector[0] = 0;
    data_vector[0] = 0;

    ui->setupUi(this);

    timer.reset(new QTimer);
    timer->setInterval(1000/10);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()) );

    for (int var = 0; var < number; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        cam_vector[var].InitCamera(var, number);
        view_vector[var]->setScene(scene_vector[var]);
    }
}

CudaSurf::~CudaSurf()
{
    delete ui;
    timer.release();
}

void CudaSurf::on_Start_clicked()
{
    timer->start();
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
            cam_vector[var].Start();
        else qDebug() << "Camera" << var << "not started";
}

void CudaSurf::on_Stop_clicked()
{
    for (int var = 0; var < number; ++var)
        cam_vector[var].Stop();
    timer->stop();
}

void CudaSurf::CaptureImage()
{
    if(cam_vector[0].Status() == XI_ON)
        for (int var = 0; var < number; var++)
        {
            mat_vector[var] = cam_vector[var].GetMatImage();

            ///////////////////////////////
            tick_vector[0] = getTickCount();

            cv::cvtColor(mat_vector[var], grey_mat_vector[var], COLOR_BGR2GRAY);
            gpumat_vector[var].upload(grey_mat_vector[var]);
            cudaSURF.Detect_cuda_surf(gpumat_vector[var], gpumat_vector[var]);

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
                qDebug() << "Usual SURF: t =" << data_vector[0];
                count_vector[0] = 0;
                sec_vector[0] = 0;
            }
            ///////////////////////////////

            gpumat_vector[var].download(mat_vector[var]);
            pixmap_vector[var] = Mat2Pixmap(mat_vector[var], true);
            view_vector[var]->scene()->clear();
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);

            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
        }
}

QPixmap CudaSurf::Mat2Pixmap(Mat data, bool isUsualMat)
{
    if (isUsualMat)
    {
        if(data.type() != 0)
        {
            // черно-белое формирование
            cv::cvtColor(data, data, COLOR_RGBA2BGRA);
            return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
        }
        // цветное формирование
        else return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_Indexed8));
    }
    // стерео формирование
    else return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_Indexed8));
}
