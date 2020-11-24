#ifndef CAM_H
#define CAM_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>

#include "xiApi.h"
#include "Sources/cameraclass.h"
#include "Sources/xiApiPlusOcv.hpp"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"

using namespace std;
using namespace cv;

namespace Ui {
class Cam;
}

class Cam : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cam(QWidget *parent = nullptr);
    QPixmap Mat2Pixmap(Mat data);
    ~Cam();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::Cam *ui;
    unique_ptr<QTimer> timer;
    QVector<XimeaCamera>           cam_vector;
    QVector<QGraphicsView*>       view_vector;
    QVector<QGraphicsScene*>     scene_vector;
    QVector<QPixmap>            pixmap_vector;
    QVector<Mat>                   mat_vector;
    xiAPIplus cam_manager;
    quint8 number;

    std::vector<Rect> faces;
    CascadeClassifier face_cascade;
};

#endif // CAM_H
