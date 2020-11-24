#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cv_qt_converter.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    Mat Matpicture;
    QPixmap img;
    Mat Matpicture_blurred;
};
#endif // MAINWINDOW_H
