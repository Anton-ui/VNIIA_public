#ifndef GRAPH_SCREEN_H
#define GRAPH_SCREEN_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class graph_screen; }
QT_END_NAMESPACE

enum class CheckStatus
{
    no_param_active,
    param1_active,
    param2_active,
    param3_active
};

class graph_screen : public QMainWindow
{
    Q_OBJECT

public:
    graph_screen(QWidget *parent = nullptr);
    ~graph_screen();

private slots:
    void on_checkBox1_clicked();
    void on_checkBox2_clicked();
    void on_checkBox3_clicked();
    void updateGraph();
    void getData(int);
    void switch_graph(int,int);

private:
    Ui::graph_screen *ui;
    QTimer dataTimer;
    CheckStatus status = CheckStatus::no_param_active;
    QVector<double> value;
    QVector<double> max_value;
    double lastFps_value;
    double now_x;
    int count_param = 3;
};
#endif // GRAPH_SCREEN_H
