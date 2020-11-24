#ifndef CUDASURF_H
#define CUDASURF_H

#include <QMainWindow>
#include <QTimer>

#include "cuda_surf.h"

using namespace std;
using namespace cv;

namespace Ui {
class CudaSurf;
}

class CudaSurf : public QMainWindow
{
    Q_OBJECT

public:
    explicit CudaSurf(QWidget *parent = 0);
    QPixmap Mat2Pixmap(Mat data, bool isUsualMat);
    ~CudaSurf();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void CaptureImage();

private:
    Ui::CudaSurf *ui;
    unique_ptr<QTimer> timer;
    QVector <XimeaCamera>       cam_vector;
    QVector <Mat>               mat_vector;
    QVector <Mat>          grey_mat_vector;
    QVector <cuda::GpuMat>   gpumat_vector;
    QVector <QPixmap>        pixmap_vector;
    QVector <QGraphicsView*>   view_vector;
    QVector <QGraphicsScene*> scene_vector;
    xiAPIplus cam_manager;
    quint8 number;

    QVector<int>      tick_vector;
    QVector<int>     count_vector;
    QVector<double>    sec_vector;
    QVector<double>   data_vector;

    cuda_surf cudaSURF;
};

#endif // CUDASURF_H
