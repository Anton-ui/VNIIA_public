#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include "cameraclass.h"

#include "xiApi.h"
#include "Sources/xiApiPlusOcv.hpp"

#include "Sources/cv_qt_converter.h"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"


namespace Ui {
class MainWindow;
}

class CameraInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraInterface(QWidget *parent = nullptr);
    ~CameraInterface();

private slots:
    void CaptureImage();

    void on_StopPushButton_clicked();
    void on_StartPushButton_clicked();
    void on_GainHorizontalSlider_valueChanged(int value);

    void on_ExposureHorizontalSlider_valueChanged(int value);
    void on_decodingSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    bool show_bool;
    std::unique_ptr<QTimer> main_timer;

    std::vector<XimeaCamera> cam_vector;
    QGraphicsPixmapItem pixmap_1;
    QGraphicsPixmapItem pixmap_2;

    xiAPIplus cam_manager;

    unsigned long cam_num;

};

#endif // MAINWINDOW_H
