#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cv_qt_converter.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QPoint>
#include <memory>
#include <QGraphicsSceneMouseEvent>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QPointF p;
signals:
    void signal2(qreal x, qreal y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        p = event->scenePos();
        qDebug() << p;
        emit signal2(p.x(), p.y());
    }
};


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void signal1();

private slots:
    void slot1();
    void slot2(qreal x, qreal y);

private:
    Ui::MainWindow *ui;
    QPixmap img;
    Mat frame;
    QTimer *timer1;
    VideoCapture cap;
    unique_ptr<CustomScene>  scene;
};

#endif // MAINWINDOW_H
