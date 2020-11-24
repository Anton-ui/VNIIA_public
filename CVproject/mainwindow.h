#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cv_qt_converter.h"
#include <mygraphicview.h>
#include <QTimer>
#include <QPoint>
#include <memory>
#include <QTcpSocket> // fortune server / fortune client

#include <QGraphicsPixmapItem>
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
    QGraphicsLineItem *item;
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
    /*void mouseMoveEvent(QGraphicsSceneMouseEvent *event){
        const QPointF p = item->mapFromScene(event->scenePos());
        QLineF l = item->line();
        l.setP2(p);
        item->setLine(l);
        qDebug() << p;
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        const QPointF p = item->mapFromScene(event->scenePos());
        QLineF l = item->line();
        l.setP2(p);
        item->setLine(l);
        qDebug() << p;*/

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void print();
    ~MainWindow();

signals:
    void signal1();

private slots:
    void slot1();

    void slot2(qreal x, qreal y);

    void slot3();

public slots:
    void on_verticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QPixmap img;
    Mat frame;
    Mat frame_blurred;
    Mat b_w_frame;
    QTimer *timer1;
    QTimer *timer2;
    VideoCapture cap;
    unique_ptr<CustomScene>  scene;
    cv::Point max_pos, min_pos;
    QString massage;
    int value_new;
    void drawEllipsesMinMax();
    double min, max;
};


#endif // MAINWINDOW_H
