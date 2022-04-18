#include "graph_windows.h"
#include "ui_graph_windows.h"

graph_windows::graph_windows(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::graph_windows)
{
    value.resize(count_param);
    max_value.resize(count_param);
    for (int i = 0; i < count_param; ++i)
        max_value[i] = 0;

    // Инициализация графики
    ui->setupUi(this);
    setWindowTitle("График параметров");
    statusBar()->clearMessage();

    // Настройка фона
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(110, 110, 110));
    gradient.setColorAt(0.75, QColor(130, 130, 130));
    gradient.setColorAt(1, QColor(150, 150, 150));
    ui->QPlot->setBackground(QBrush(gradient));

    // Настройка графиков: у:
    for (int j = 0; j < count_param; ++j)
    {
        ui->QPlot->addGraph();
        ui->QPlot->graph(j)->setPen(QColor(255, 60, 40));
    }

    // Настройка графиков: х (время):
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->QPlot->xAxis->setTicker(timeTicker);
    //ui->QPlot->axisRect()->setupFullAxesBox();

    // настройка функции изменения границ координат графика:
    connect(ui->QPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->QPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->QPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->QPlot->yAxis2, SLOT(setRange(QCPRange)));

    // настройка FPS и запуск графика
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateGraph()));
    dataTimer.start(50);
}

graph_windows::~graph_windows()
{
    delete ui;
}

void graph_windows::updateGraph()
{
    // Настройка констант и параметров
    static QTime time(QTime::currentTime());
    static const double start_const = time.minute() * 60 + time.hour() * 3600;
    static int last_var = -1;
    static int var = -1;

    // Отслеживание того, какой график сейчас активен
    if (status == CheckStatus::param1_active) {var = 0;}
    else if (status == CheckStatus::param2_active) {var = 1;}
    else if (status == CheckStatus::param3_active) {var = 2;}
    else var = -1;

    now_x = (time.elapsed() / 1000.0) + start_const;
    if (var != last_var) {switch_graph(var, last_var);}
    if (var != -1)
    {
        getData(var);

        // calculate FPS:
        static int frameCount;
        ++frameCount;
        ui->statusBar->showMessage(QString("%1 FPS, Total Data points: %2")
              .arg(frameCount/(now_x-lastFps_value), 0, 'f', 0)
              .arg(ui->QPlot->graph(var)->data()->size()), 0);
        lastFps_value = now_x;
        frameCount = 0;
    }
    last_var = var;
}

void graph_windows::getData(int num)
{
    // Настройка чтения пакетов
    if (read_status == ReadStatus::read_10_12) {bytes_start = 10;}
    else if (read_status == ReadStatus::read_13_14) {bytes_start = 13;}
    else bytes_start = 0;

    // Получение данных
    // value[num] = get_from_port(..., bytes_start, ...);
    value[num] = qSin(now_x)+qrand()/(double)RAND_MAX*1*qSin(now_x/0.3843) + 1.8; // рандом-генерация
    ui->QPlot->graph(num)->addData(now_x, value[num]);
    qDebug() << bytes_start;

    // Повышение максимума на оси у
    if (value[num] > max_value[num]) {
        max_value[num] = value[num];
        ui->QPlot->yAxis->setRange(0, max_value[num] + 1);
    }

    // Обновление графика
    ui->QPlot->xAxis->setRange(now_x, 20, Qt::AlignRight); // 8 - демонстрируемый диапазон времени на графике
    ui->QPlot->replot();
}

void graph_windows::switch_graph(int var_new, int var_old)
{
    if (var_old != -1) {
        ui->QPlot->graph(var_old)->addData(now_x, 0);
        ui->QPlot->graph(var_old)->setVisible(false);
    }
    if (var_new != -1) {
        ui->QPlot->graph(var_new)->setVisible(true);
        ui->QPlot->graph(var_new)->addData(now_x, 0);
        ui->QPlot->yAxis->setRange(0, max_value[var_new] + 1);
    }
}

void graph_windows::on_Parametr_1_triggered()
{
    if (ui->Parametr_1->isChecked())
    {
        ui->Parametr_2->setChecked(false);
        ui->Parametr_3->setChecked(false);
        status = CheckStatus::param1_active;
    } else status = CheckStatus::no_param_active;
}

void graph_windows::on_Parametr_2_triggered()
{
    if (ui->Parametr_2->isChecked())
    {
        ui->Parametr_1->setChecked(false);
        ui->Parametr_3->setChecked(false);
        status = CheckStatus::param2_active;
    } else status = CheckStatus::no_param_active;
}

void graph_windows::on_Parametr_3_triggered()
{
    if (ui->Parametr_3->isChecked())
    {
        ui->Parametr_1->setChecked(false);
        ui->Parametr_2->setChecked(false);
        status = CheckStatus::param3_active;
    } else status = CheckStatus::no_param_active;
}

void graph_windows::on_Network_1_triggered()
{
    if (ui->Network_1->isChecked())
    {
        ui->Network_2->setChecked(false);
        read_status = ReadStatus::read_10_12;
    } else read_status = ReadStatus::read_all;
}

void graph_windows::on_Network_2_triggered()
{
    if (ui->Network_2->isChecked())
    {
        ui->Network_1->setChecked(false);
        read_status = ReadStatus::read_13_14;
    } else read_status = ReadStatus::read_all;
}
