#ifndef GRAPH_WINDOWS_H
#define GRAPH_WINDOWS_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class graph_windows; }
QT_END_NAMESPACE

enum class CheckStatus
{
    no_param_active,
    param1_active,
    param2_active,
    param3_active
};

enum class ReadStatus
{
    read_all,
    read_10_12,
    read_13_14
};

class graph_windows : public QMainWindow
{
    Q_OBJECT

public:
    graph_windows(QWidget *parent = nullptr);
    ~graph_windows();

private slots:
    void updateGraph();
    void getData(int);
    void switch_graph(int,int);
    void on_Parametr_1_triggered();
    void on_Parametr_2_triggered();
    void on_Parametr_3_triggered();
    void on_Network_1_triggered();
    void on_Network_2_triggered();

private:
    Ui::graph_windows *ui;
    QTimer dataTimer;
    CheckStatus status = CheckStatus::no_param_active;
    ReadStatus read_status = ReadStatus::read_all;
    QVector<double> value;
    QVector<double> max_value;
    double lastFps_value;
    double now_x;
    int count_param = 3;
    int bytes_start;
};
#endif // GRAPH_WINDOWS_H
