#ifndef CAM_H
#define CAM_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>

#include "Sources/cameraclass.h"
#include "xiApi.h"
#include "Sources/xiApiPlusOcv.hpp"
#include "Sources/cv_qt_converter.h"

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
    ~Cam();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::Cam *ui;
    QVector<XimeaCamera>       cam_vector;
    QVector<QGraphicsView*>   view_vector;
    QVector<QPixmap>        pixmap_vector;
    QVector<Mat>               mat_vector;
    QVector<QGraphicsScene*> scene_vector;
    unique_ptr<QTimer> timer;
    xiAPIplus cam_manager;
    quint8 number;
};

#endif // CAM_H
