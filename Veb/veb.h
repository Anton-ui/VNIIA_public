#ifndef VEB_H
#define VEB_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <memory>

#include "cv_qt_converter.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

namespace Ui {
class Veb;
}

class Veb : public QMainWindow
{
    Q_OBJECT

public:
    explicit Veb(QWidget *parent = nullptr);
    ~Veb();

private slots:
    void on_Start_clicked();
    void on_Stop_clicked();
    void SlotCapture();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Veb *ui;
    unique_ptr<QTimer> timer;
    unique_ptr<QGraphicsScene> scene;
    VideoCapture cap;
    QPixmap pix;
    Mat frame;
    int value;
};

#endif // VEB_H
