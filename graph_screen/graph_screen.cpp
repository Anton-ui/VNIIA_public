#include "graph_screen.h"
#include "ui_graph_screen.h"

graph_screen::graph_screen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::graph_screen)
{
    value.resize(count_param);
    max_value.resize(count_param);
    for (int var = 0; var < count_param; ++var)
        max_value[var] = 0;

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
    for (int var = 0; var < count_param; ++var)
    {
        ui->QPlot->addGraph();
        ui->QPlot->graph(var)->setPen(QColor(255, 60, 40));
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

graph_screen::~graph_screen()
{
    delete ui;
}

void graph_screen::updateGraph()
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

void graph_screen::getData(int var)
{
    // Получение данных
    //value[var] = get_from_port(...);
    value[var] = qSin(now_x)+qrand()/(double)RAND_MAX*1*qSin(now_x/0.3843) + 1.5; // рандом-генерация
    ui->QPlot->graph(var)->addData(now_x, value[var]);

    // Повышение максимума на оси у
    if (value[var] > max_value[var]) {
        max_value[var] = value[var];
        ui->QPlot->yAxis->setRange(0, max_value[var] + 1);
    }

    // Обновление графика
    ui->QPlot->xAxis->setRange(now_x, 20, Qt::AlignRight); // 8 - демонстрируемый диапазон времени на графике
    ui->QPlot->replot();
}

void graph_screen::switch_graph(int var_new, int var_old)
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

void graph_screen::on_checkBox1_clicked()
{
    if (ui->checkBox1->isChecked())
    {
        ui->checkBox2->setCheckState(Qt::Unchecked);
        ui->checkBox3->setCheckState(Qt::Unchecked);
        status = CheckStatus::param1_active;
    } else status = CheckStatus::no_param_active;
}

void graph_screen::on_checkBox2_clicked()
{
    if (ui->checkBox2->isChecked())
    {
        ui->checkBox1->setCheckState(Qt::Unchecked);
        ui->checkBox3->setCheckState(Qt::Unchecked);
        status = CheckStatus::param2_active;
    } else status = CheckStatus::no_param_active;
}

void graph_screen::on_checkBox3_clicked()
{
    if (ui->checkBox3->isChecked())
    {
        ui->checkBox1->setCheckState(Qt::Unchecked);
        ui->checkBox2->setCheckState(Qt::Unchecked);
        status = CheckStatus::param3_active;
    } else status = CheckStatus::no_param_active;
}
