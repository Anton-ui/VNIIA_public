#ifndef CAM_H
#define CAM_H

#include <QMainWindow>
#include <QTimer>

#include "Sources/cv_qt_converter.h"

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
    void realtimeDataSlot();

private:
    Ui::cam *ui;
    QVector<XimeaCamera>       cam_vector;
    QVector<QGraphicsView*>   view_vector;
    QVector<QPixmap>        pixmap_vector;
    QVector<Mat>               mat_vector;
    QVector<bool>           switch_vector;
    QVector<QGraphicsScene*> scene_vector;
    unique_ptr<QTimer> timer;
    xiAPIplus cam_manager;
};

#endif // CAM_H
