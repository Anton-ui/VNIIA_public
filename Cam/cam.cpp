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

    timer.reset(new QTimer);
    timer->setInterval(1000/30);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    ui->setupUi(this);
    for (int var = 0; var < number; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        cam_vector[var].InitCamera(var, number);
        view_vector[var]->setScene(scene_vector[var]);
    }
    //face_cascade.load("/home/camel212/QtProjects/haarcascade_frontalface_alt.xml");
}

Cam::~Cam()
{
    delete ui;
    timer.release();
}

void Cam::on_Start_clicked()
{
    if(cam_vector[0].Status() != XI_ON)
        for (int var = 0; var < cam_vector.size(); ++var)
           cam_vector[var].Start();
    timer->start();
}

void Cam::on_Stop_clicked()
{
    timer->stop();
    for (int var = 0; var < cam_vector.size(); ++var)
    {
       cam_vector[var].Stop();
    }
}

void Cam::CaptureImage()
{
    if(cam_vector[0].Status() == XI_ON)
        for (int var = 0; var < number; var++)
        {
            //pixmap_vector[var].loadFromData(cam_vector[var].GetUcharImage());
            mat_vector[var] = cam_vector[var].GetMatImage();
            view_vector[var]->scene()->clear();

            /*face_cascade.detectMultiScale(mat_vector[var], faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
            for (int v = 0; v < faces.size(); var++)
            {
                view_vector[var]->scene()->addEllipse(faces[v].x + faces [v].width*0.5, faces[v].y + faces[v].height*0.5, 5, 5, pen_red);
            }*/

            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);
            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
        }
}

QPixmap Cam::Mat2Pixmap(Mat data)
{
    cvtColor(data, data, COLOR_RGBA2BGRA);
    return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
}
