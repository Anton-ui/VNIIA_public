#ifndef CAM_H
#define CAM_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "Sources/cv_qt_converter.h"
#include "method_akaze.h"
#include "method_surf.h"
#include "method_orb.h"

using namespace std;
using namespace cv;

namespace Ui {
class cam;
}

class cam : public QMainWindow
{
    Q_OBJECT

public:
    explicit cam(QWidget *parent = nullptr);
    QPixmap Mat2Pixmap(Mat data);
    ~cam();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

    void on_check_orb_stateChanged   (int switch2);
    void on_check_surf_stateChanged  (int switch3);
    void on_check_akaze_stateChanged (int switch4);

private:
    Ui::cam *ui;
    QVector<XimeaCamera>       cam_vector;
    QVector<QGraphicsView*>   view_vector;
    QVector<QPixmap>        pixmap_vector;
    QVector<Mat>               mat_vector;
    QVector<bool>           switch_vector;
    QVector<QGraphicsScene*> scene_vector;
    QVector<int>      tick_vector;
    QVector<int>     count_vector;
    QVector<double>    sec_vector;
    QVector<double>   data_vector;
    QVector<double>         ticks;
    QVector<QString>       labels;
    unique_ptr<QTimer> timer;
    xiAPIplus cam_manager;
    quint8 number;

    method_orb       orb;
    method_surf     surf;
    method_akaze   akaze;
};

#endif // CAM_H
