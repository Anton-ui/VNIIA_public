#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inireader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString path = "/home/anton/Qtprojects/ini.ini";
    ~MainWindow();

private slots:
    void on_SaveFile_clicked();

    void on_pathButton_clicked();

    void on_changeButton_clicked();

    void on_addButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    IniReader inireader;
};

#endif // MAINWINDOW_H
