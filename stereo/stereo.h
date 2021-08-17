#ifndef STEREO_H
#define STEREO_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include "Sources/cameraclass.h"

//#include "opencv2/cudafeatures2d.hpp"
//#include "opencv2/cudaimgproc.hpp"
//#include "opencv2/cudaobjdetect.hpp"
//#include <opencv2/cudastereo.hpp>

#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/calib3d.hpp"

#include "opencv2/core/cuda.hpp"

using namespace std;
using namespace cv;

namespace Ui {
class stereo;
}

class stereo : public QMainWindow
{
    Q_OBJECT

public:
    explicit stereo(QWidget *parent = 0);
    QPixmap Mat2Pixmap(Mat data, bool isUsualMat);
    ~stereo();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void on_Calibration_clicked();

    void CaptureImage();
    void on_Save_clicked();
    void on_Load_clicked();

    void on_horizontalSlider_sliderMoved(int position);
    void on_horizontalSlider_2_sliderMoved(int position);
    void on_horizontalSlider_3_sliderMoved(int position);
    void on_horizontalSlider_4_sliderMoved(int position);
    void on_horizontalSlider_5_sliderMoved(int position);
    void on_horizontalSlider_6_sliderMoved(int position);
    void on_horizontalSlider_7_sliderMoved(int position);
    void on_horizontalSlider_8_sliderMoved(int position);
    void on_horizontalSlider_9_sliderMoved(int position);

private:

    Size patternsize;
    float square_size;
    void PerformCalibration();
    int calib_counter = 0;
    Ui::stereo *ui;
    unique_ptr<QTimer> timer;
    QVector<XimeaCamera>        cam_vector;
    QVector<QGraphicsView*>    view_vector;
    QVector<QGraphicsScene*>  scene_vector;
    QVector<QPixmap>         pixmap_vector;
    QVector<Mat>                mat_vector;
    QVector<Mat>           grey_mat_vector;
    QVector<Mat>          undistort_vector;
    xiAPIplus cam_manager;
    quint8 number;

    Ptr<StereoBM> sbm;
    Mat disp;
    Mat disp8;

    bool calibration = false;
    bool done = false;

    vector <vector <Point3f> > object_points;
    vector <vector <Point2f> > imagePoints1, imagePoints2;
    Mat map1x;
    Mat map1y;
    Mat map2x;
    Mat map2y;

    Mat CM1 = Mat(3, 3, CV_64F);
    Mat CM2 = Mat(3, 3, CV_64F);
    Mat D1 = Mat(1, 5, CV_64F);
    Mat D2 = Mat(1, 5, CV_64F);
    Mat R = Mat(3, 3, CV_64F);
    Mat T = Mat(3, 1, CV_64F);
    Mat E = Mat(3, 3, CV_64F);
    Mat F = Mat(3, 3, CV_64F);

    Mat R1 = Mat(3, 3, CV_64F);
    Mat R2 = Mat(3, 3, CV_64F);
    Mat P1 = Mat(3, 4, CV_64F);
    Mat P2 = Mat(3, 4, CV_64F);
    Mat Q = Mat(4, 4, CV_64F);
};

#endif // STEREO_H
