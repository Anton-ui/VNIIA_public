#ifndef CAM_H
#define CAM_H

#define TRUE 1
#define FALSE 0

#define IS_UI           TRUE
#define IS_DETECT_FACE FALSE


#include <QMainWindow>
#include <QTimer>

// Графическая оболочка
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "Sources/cameraclass.h"

using namespace std;
using namespace cv;


QT_BEGIN_NAMESPACE
namespace Ui { class Cam; }
QT_END_NAMESPACE

class Cam : public QMainWindow
{
    Q_OBJECT

public:
    Cam(QWidget *parent = nullptr);
    ~Cam();
    QPixmap Mat2Pixmap(Mat data);

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::Cam *ui;
    unique_ptr<QTimer> timer;
    QVector<XimeaCamera> cam_vector;
    QVector<Mat>         mat_vector;
    xiAPIplus cam_manager;
    quint8 number;

    // Способ получения изображения через QByteArray
    QVector<QByteArray> img_in_bytes;

    // Обнаружение лиц на камере
    vector<Rect> faces;
    CascadeClassifier face_cascade;

    // Графическая оболочка
    QVector<QGraphicsView*>   view_vector;
    QVector<QGraphicsScene*> scene_vector;
    QVector<QPixmap>        pixmap_vector;
    bool is_started = false;
};
#endif // CAM_H
