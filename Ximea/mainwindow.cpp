#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cap(0)
{
    ui->setupUi(this);
    timer1 = new QTimer();
    timer1->setInterval(1000/30);
    connect(timer1, SIGNAL(timeout()),this, SLOT(slot1()));
    timer1->start();

    scene.reset(new CustomScene);
    ui->graphicView->setScene(scene.get());
    connect(scene.get(), &CustomScene::signal2,this, &MainWindow::slot2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot1()
{
    cap >> frame;
    img = cvMatToQPixmap(frame);
    scene->addPixmap(img);
    ui->graphicView->setScene(scene.get());
    ui->graphicView->fitInView(img.rect(), Qt::KeepAspectRatio);
}

void MainWindow::slot2(qreal x, qreal y)
{
    qDebug() << x << y;
}
