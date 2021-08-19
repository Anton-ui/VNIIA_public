#include "cam.h"
#include "ui_cam.h"

cam::cam(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cam)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    pixmap_vector.resize(number+1);
    mat_vector.resize(number);

    timer.reset(new QTimer);
    timer->setInterval(1000/30);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    ui->setupUi(this);

    for (int var = 0; var < number+1; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        view_vector[var]->setScene(scene_vector[var]);
        if (var != number)
        {
            cam_vector[var].InitCamera(var, number);
        }
    }
}

cam::~cam()
{
    delete ui;
    timer.release();
}


void cam::on_Start_clicked()
{
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
           cam_vector[var].Start();
    timer->start();
}


void cam::on_Stop_clicked()
{
    timer->stop();
    for (int var = 0; var < number; ++var)
       cam_vector[var].Stop();
}

void cam::CaptureImage()
{
    for (int var = 0; var < number; var++)
        if(cam_vector[var].Status() == XI_ON)
        {
            view_vector[var]->scene()->clear();
            mat_vector[var] = cam_vector[var].GetMatImage();
            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);
            see(var);

            process_view(mat_vector[var]);
        }
}

QPixmap cam::Mat2Pixmap(Mat data)
{
    cvtColor(data, data, COLOR_RGBA2BGRA);
    return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
}

void cam::see(int var)
{
    view_vector[var]->scene()->addPixmap(pixmap_vector[var]);
    view_vector[var]->setScene(scene_vector[var]);
    view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
}

void cam::process_view(Mat incoming_mat)
{
    Mat src_gray, gray2;
    Mat dst, detected_edges;
    int lowThreshold = 3000;
    const int ratio = 3;
    const int kernel_size = 7;

    cv::cvtColor(incoming_mat, src_gray, CV_BGR2GRAY);
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    pixmap_vector[1] = Mat2Pixmap(detected_edges);
    see(1);
}
