#include "mainwindow.h"
#include "inireader.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    inireader.SetPath(path);
    delete ui;
}

void MainWindow::on_pathButton_clicked()
{
    QString path_in = ui->line->text();
    inireader.SetPath(path_in);
}

void MainWindow::on_changeButton_clicked()
{
    QString group = ui->line2->text();
    QString   key = ui->line3->text();
    QString value = ui->line4->text();
    inireader.ChangeValue(group, key, value);
}

void MainWindow::on_addButton_clicked()
{
    QString group = ui->line2->text();
    QString   key = ui->line3->text();
    QString value = ui->line4->text();
    inireader.AddValue(group, key, value);
}

void MainWindow::on_clearButton_clicked()
{
    QString group = ui->line5->text();
    QString   key = ui->line6->text();
    inireader.ClearValue(group, key);
}

void MainWindow::on_SaveFile_clicked()
{
    QString path_in = ui->line->text();
    inireader.SaveFile(path_in);
}
