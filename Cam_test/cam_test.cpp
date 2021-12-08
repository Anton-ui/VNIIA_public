#include "cam_test.h"
#include "ui_cam.h"

/* Данный проект - основа для работы с камерами Ximea, в нем упорядочены основные принципы.
Камеры Ximea могут быть usb 3.0 с разрешениями: 1032 772, 2064 1564
Камеры Ximea usb 2.0 имеют следующие разрешения: 368 276, 432 324, 648 486, 1296 972
Как правило, камеры usb 2.0 имеют номер модели №23, что активно используется в проектах,
так как для них нужно часто прописывать условия работы отдельно.
*/

Cam::Cam(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cam)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    pixmap_vector.resize(number);
    mat_vector.resize(number);

    timer.reset(new QTimer);
    timer->setInterval(1000/FPS);
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
}

Cam::~Cam()
{
    delete ui;
    timer.release();
}


void Cam::on_Start_clicked()
{
    for (int var = 0; var < cam_vector.size(); ++var)
        if(cam_vector[var].Status() != XI_ON)
           cam_vector[var].Start();
    timer->start();
}


void Cam::on_Stop_clicked()
{
    //timer->stop();
    for (int var = 0; var < cam_vector.size(); ++var)
       cam_vector[var].Stop();
    is_started = false;
}

void Cam::CaptureImage()
{
    for (int var = 0; var < number; var++)
        if(cam_vector[var].Status() == XI_ON)
        {
            view_vector[var]->scene()->clear();

            // Способ получения изображения через Mat
            mat_vector[var] = cam_vector[var].GetMatImage();
            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);

            // Способ получения изображения через QByteArray
            //img_in_bytes[var] = cam_vector[var].GetUcharImage();
            //pixmap_vector[var].QPixmap::loadFromData(img_in_bytes[var]);

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
