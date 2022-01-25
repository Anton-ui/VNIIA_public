#include "cam.h"
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
    if (IS_DETECT_FACE)
        face_cascade.load("/home/camel212/QtProjects/haarcascade_frontalface_alt.xml");
    timer->start();
}

Cam::~Cam()
{
    delete ui;
    timer.release();
}


void Cam::on_Start_clicked()
{
    if (!is_started)
    {
        for (int var = 0; var < cam_vector.size(); ++var)
            if(cam_vector[var].Status() != XI_ON)
               cam_vector[var].Start();
        //timer->start();
        is_started = true;
    }
    // Узнать версию opencv:
    //qDebug() << "version:" << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION;
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
            if (IS_UI) // Графическая оболочка
                view_vector[var]->scene()->clear();

            // Способ получения изображения через Mat
            mat_vector[var] = cam_vector[var].GetMatImage();
            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);

            // Способ получения изображения через QByteArray
            //img_in_bytes[var] = cam_vector[var].GetUcharImage();
            //pixmap_vector[var].QPixmap::loadFromData(img_in_bytes[var]);

            // Обнаружение лиц на камере
            if (IS_DETECT_FACE)
            {
                // Реализация устрела, при необходимости нужно будет ее обновить
                /*face_cascade.detectMultiScale(mat_vector[var], faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
                for (int v = 0; v < faces.size(); var++)
                    view_vector[var]->scene()->addEllipse(
                                faces[v].x + faces [v].width*0.5, faces[v].y + faces[v].height*0.5, 5, 5, pen_red);*/
            }

            if (IS_UI) // Графическая оболочка
            {
                if (IS_GRANI)
                {
                    process_view(mat_vector[var], var);
                }
                view_vector[var]->scene()->addPixmap(pixmap_vector[var]);
                view_vector[var]->setScene(scene_vector[var]);
                view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
            }
        }
}

QPixmap Cam::Mat2Pixmap(Mat data)
{
    cvtColor(data, data, COLOR_RGBA2BGRA);
    return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
}

// Наработка по обнаружению границ
void Cam::process_view(Mat incoming_mat, int var)
{
    Mat src_gray, gray2;
    Mat dst, detected_edges;
    int lowThreshold = 3000;
    const int ratio = 3;
    const int kernel_size = 7;

    cv::cvtColor(incoming_mat, src_gray, CV_BGR2GRAY);
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    pixmap_vector[var] = Mat2Pixmap(detected_edges);
}



// Наработка по сдвигу изображения
/*Mat Cam::translateImg(Mat &img, int offsetx, int offsety)
{
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
    return img;
}

translateImg(mat_vector[var],-200, 0);
translateImg(mat_vector[var], 100, 0);
translateImg(mat_vector[var], 0, 200);
translateImg(mat_vector[var], 0, -50);

addWeighted( mat_vector[var], alpha, black, beta, 0.0, result); наложение картинок со своим весом*/


// Наработка по использованию вертикального ползунка в графическом редакторе
/*void Cam::on_verticalSlider_valueChanged(int value)
{
    GaussianBlur(Mat_picture, Mat_picture_blurred, Size(7, 7), value);
}
*/


// Наработка по использованию ручки в графическом редакторе
/*QPen pen_blue(Qt::blue);
pen_red.setWidth(10);
scene->addEllipse(min_pos.x, min_pos.y, 5, 5, pen_red);


// ~~~Наработка по рисованию на выходящем изображении
void Cam::drawEllipsesMinMax()
{
    cvtColor(frame, b_w_frame, CV_BGR2GRAY);
    blur(b_w_frame, frame_blurred, Size(10,10));
    minMaxLoc(frame_blurred, &min, &max, &min_pos, &max_pos);

    if( (min_pos.x > 10) && (min_pos.x < frame.cols-10) && (min_pos.y > 10) && (min_pos.y < frame.rows - 10) )
        scene->addEllipse(min_pos.x, min_pos.y, 5, 5, pen_blue);
    if( (max_pos.x > 10) && (max_pos.x < frame.cols-10) && (max_pos.y > 10) && (max_pos.y < frame.rows - 10) )
        scene->addEllipse(max_pos.x, max_pos.y, 5, 5, pen_red);
}
*/


// Наработка по использованию мышки в графическом редакторе
/*QPointF p;
signals:
    void signal2(qreal x, qreal y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        p = event->scenePos();
        qDebug() << p;
        emit signal2(p.x(), p.y());
    }
*/


// Наработка по использованию мыши с widget
/*
class CustomScene : public QGraphicsScene
{
    Q_OBJECT
    QGraphicsLineItem *item;
public:
    QPointF p;
signals:
    void signal2(qreal x, qreal y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        p = event->scenePos();
        qDebug() << p;
        emit signal2(p.x(), p.y());
    }
};

unique_ptr<CustomScene> scene;   // custom!!
        scene.reset(new CustomScene);
        view->setScene(scene.get());
*/
