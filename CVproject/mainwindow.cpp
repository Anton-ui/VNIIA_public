#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsBlurEffect>

using namespace std;

QPen pen_red(Qt::red);
QPen pen_blue(Qt::blue);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cap(0),
    value_new(0)
{
    ui->setupUi(this);

    pen_red.setWidth(10);
    pen_blue.setWidth(10);

    timer1 = new QTimer();
    timer1->setInterval(1000/30);
    connect(timer1, SIGNAL(timeout()),this, SLOT(slot1()));
    timer1->start();

    timer2 = new QTimer();
    timer2->setInterval(500);
    connect(timer2, SIGNAL(timeout()),this, SLOT(slot3()));
    timer2->start();

    scene.reset(new CustomScene);
    ui->graphicView->setScene(scene.get());
    //ui->graphicView->scene()->addItem(&pixmap_item);
    connect(scene.get(), &CustomScene::signal2,this, &MainWindow::slot2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot3()
{
    drawEllipsesMinMax();
    print();
}
void MainWindow::slot2(qreal x, qreal y)
{
    qDebug() << x << y;
}

void MainWindow::slot1()
{
    cap >> frame;
    if (value_new == 0)
        img = cvMatToQPixmap(frame);
    else {
        GaussianBlur(frame, frame, Size(7, 7), value_new);
        img = cvMatToQPixmap(frame);
    }
    scene->addPixmap(img);
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    value_new = value;
}

void MainWindow::drawEllipsesMinMax()
{
    scene->clear();
    cvtColor(frame, b_w_frame, CV_BGR2GRAY);
    blur(b_w_frame, frame_blurred, Size(10,10));
    minMaxLoc(frame_blurred, &min, &max, &min_pos, &max_pos);
    scene->addPixmap(img);

    if( (min_pos.x > 10) && (min_pos.x < frame.cols-10) && (min_pos.y > 10) && (min_pos.y < frame.rows - 10) )
        scene->addEllipse(min_pos.x, min_pos.y, 5, 5, pen_blue);
    if( (max_pos.x > 10) && (max_pos.x < frame.cols-10) && (max_pos.y > 10) && (max_pos.y < frame.rows - 10) )
        scene->addEllipse(max_pos.x, max_pos.y, 5, 5, pen_red);

    ui->graphicView->setScene(scene.get());
    ui->graphicView->fitInView(img.rect(), Qt::KeepAspectRatio);
}

void MainWindow::print()
{
    massage = "";
    massage.append(QString::number(min));
    massage.append("\n");
    massage.append(QString::number(min_pos.x));
    massage.append(" ");
    massage.append(QString::number(min_pos.y));
    massage.append("\n");
    massage.append(QString::number(max));
    massage.append("\n");
    massage.append(QString::number(max_pos.x));
    massage.append(" ");
    massage.append(QString::number(max_pos.y));
    ui->label->setText(massage);
}
