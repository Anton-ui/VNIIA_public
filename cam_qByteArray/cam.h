#ifndef CAM_H
#define CAM_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <typeinfo>

#include "Sources/cameraclass.h"
#include "Sources/cv_qt_converter.h"

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class cam; }
QT_END_NAMESPACE

class cam : public QMainWindow
{
    Q_OBJECT

public:
    cam(QWidget *parent = nullptr);
    ~cam();
    QPixmap Mat2Pixmap(Mat data);

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();
    void process_view(Mat incoming_mat);
    void see(int v);

private:
    Ui::cam *ui;
    unique_ptr<QTimer> timer;
    QVector<XimeaCamera>        cam_vector;
    QVector<QGraphicsView*>    view_vector;
    QVector<QGraphicsScene*>  scene_vector;
    QVector<QPixmap>         pixmap_vector;
    QVector<Mat>                mat_vector;
    xiAPIplus cam_manager;
    quint8 number;
};
#endif // CAM_H
