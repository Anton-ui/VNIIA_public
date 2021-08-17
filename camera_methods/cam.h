#ifndef CAM_H
#define CAM_H

#define CORE 12

#include <QMainWindow>
#include <QThread>
#include <QTimer>

#include "Sources/cv_qt_converter.h"
#include "Sources/qcustomplot.h"
#include "threads_manager.h"

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

    void on_check_template_stateChanged (int switch1);
    void on_check_xml_stateChanged      (int switch2);
    void on_check_surf_stateChanged     (int switch3);
    void on_check_akaze_stateChanged    (int switch4);

signals:
    void data(int in_var, Mat &mat_vector, QPixmap &pixmap_vector, QGraphicsView &view_vector, QGraphicsScene &scene_vector,
              QVector<bool> switch_vector, QPen pen);
    void frameFinished();

private:
    Ui::cam *ui;
    void update();
    QVector<QThread*>          thread_vector;
    QVector<threads_manager*> manager_vector;
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

    bool calculate = true;
    QCPBars *graph;
    double max_val_graph = 210;
    double max_val_graph2 = 250;
    double constanta;
    int flag = 0;

    method_xml       xml;
    method_surf     surf;
    method_template temp;
    method_akaze   akaze;
};

#endif // CAM_H
