#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path = "/home/anton/3.jpeg";
    Matpicture = imread(path.toStdString());
    GaussianBlur(Matpicture, Matpicture_blurred, Size(7, 7), 7);

    img = cvMatToQPixmap(Matpicture);
    //img = cvMatToQPixmap(Matpicture_blurred);

    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(img);
    ui->graphicView->setScene(scene);
    //ui->graphicView->fitInView(img.rect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->graphicView->scene()->clear();
    if (value == 0)
        img = cvMatToQPixmap(Matpicture);
    else {
        GaussianBlur(Matpicture, Matpicture_blurred, Size(7, 7), value);
        img = cvMatToQPixmap(Matpicture_blurred);
    }
    ui->graphicView->scene()->addPixmap(img);
}
