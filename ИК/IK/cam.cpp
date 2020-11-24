#include "cam.h"
#include "ui_cam.h"

cam::cam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cam)
{
    cam_vector.resize(1);
    pixmap_vector.resize(1);
    mat_vector.resize(1);

    ui->setupUi(this);
    timer.reset(new QTimer);
    timer->setInterval(1000/30);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    for (int var = 0; var < number; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        cam_vector[var].InitCamera(var, number);
        view_vector[var]->setScene(scene_vector[var]);
    }
}

cam::~cam()
{
    timer.release();
    delete ui;
}

void cam::on_Start_clicked()
{
    timer->start();
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
        {
            cam_vector[var].Start();
            //thread_vector[var]->start();
        }
        else qDebug() << "Camera" << var << "not started";
}

void cam::on_Stop_clicked()
{
    for (int var = 0; var < number; ++var)
        cam_vector[var].Stop();
    timer->stop();
}

void cam::CaptureImage()
{ 
    for (int var = 0; var < number; var++)
        if(cam_vector[var].Status() == XI_ON)
        {
            //pixmap_vector[var].loadFromData(cam_vector[var].GetUcharImage());

            tick_vector[8] = getTickCount();
            mat_vector[var] = cam_vector[var].GetMatImage();
            view_vector[var]->scene()->clear();

            if (switch_vector[0])
            {
                if ((calculate) && (var == 0))
                {
                    tick_vector[0] = getTickCount();
                    temp.Method_temp_ON(mat_vector[var], mat_vector[var]);
                    tick_vector[1] = getTickCount();
                    double secs = (tick_vector[1] - tick_vector[0]) / getTickFrequency();
                    if(count_vector[0] < 100)
                    {
                        sec_vector[0] += secs;
                        count_vector[0]++;
                    }
                    else
                    {
                        data_vector[0] = sec_vector[0] / 100;
                        qDebug() << "Template: t =" << data_vector[0];
                        count_vector[0] = 0;
                        sec_vector[0] = 0;
                    }
                }
                else temp.Method_temp_ON(mat_vector[var], mat_vector[var]);
            }

            if (switch_vector[1])
            {
                if ((calculate) && (var == 0))
                {
                    tick_vector[2] = getTickCount();
                    xml.Method_xml_ON(mat_vector[var], mat_vector[var]);
                    tick_vector[3] = getTickCount();
                    double secs = (tick_vector[3] - tick_vector[2]) / getTickFrequency();
                    if(count_vector[1] < 100)
                    {
                        sec_vector[1] += secs;
                        count_vector[1]++;
                    }
                    else
                    {
                        data_vector[1] = sec_vector[1] / 100;
                        qDebug() << "XML: t =" << data_vector[1];
                        count_vector[1] = 0;
                        sec_vector[1] = 0;
                    }
                }
                else xml.Method_xml_ON(mat_vector[var], mat_vector[var]);
            }

            if (switch_vector[2])
            {
                if ((calculate) && (var == 0))
                {
                    tick_vector[4] = getTickCount();
                    surf.Method_surf_ON(mat_vector[var], mat_vector[var]);
                    tick_vector[5] = getTickCount();
                    double secs = (tick_vector[5] - tick_vector[4]) / getTickFrequency();
                    if(count_vector[2] < 100)
                    {
                        sec_vector[2] += secs;
                        count_vector[2]++;
                    }
                    else
                    {
                        data_vector[2] = sec_vector[2] / 100;
                        qDebug() << "SURF: t =" << data_vector[2];
                        count_vector[2] = 0;
                        sec_vector[2] = 0;
                    }
                }
                else surf.Method_surf_ON(mat_vector[var], mat_vector[var]);
            }

            if (switch_vector[3])
            {
                if ((calculate) && (var == 0))
                {
                    tick_vector[6] = getTickCount();
                    akaze.Method_akaze_ON(mat_vector[var], mat_vector[var]);
                    tick_vector[7] = getTickCount();
                    double secs = (tick_vector[7] - tick_vector[6])/getTickFrequency();
                    if(count_vector[3] < 100)
                    {
                        sec_vector[3] += secs;
                        count_vector[3]++;
                    }
                    else
                    {
                        data_vector[3] = sec_vector[3] / 100;
                        qDebug() << "AKAZE: t =" << data_vector[3];
                        count_vector[3] = 0;
                        sec_vector[3] = 0;
                    }
                }
                else akaze.Method_akaze_ON(mat_vector[var], mat_vector[var]);
            }

            pixmap_vector[var] = Mat2Pixmap(mat_vector[var]);
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);
            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);

            if ((calculate) && (var == 0))
            {
                tick_vector[9] = getTickCount();
                double s = (tick_vector[9] - tick_vector[8]) / getTickFrequency();
                data_vector[4] = s;
                update();
            }
        }
}

void cam::on_check_template_stateChanged(int switch1)
{
    if (switch1 != 0)
        switch_vector[0] = true;
    else switch_vector[0] = false;
}

void cam::on_check_xml_stateChanged(int switch2)
{
    if (switch2 != 0)
        switch_vector[1] = true;
    else switch_vector[1] = false;
}

void cam::on_check_surf_stateChanged(int switch3)
{
    if (switch3 != 0)
        switch_vector[2] = true;
    else switch_vector[2] = false;

    //thread_manager.update_proc_info;
}

void cam::on_check_akaze_stateChanged(int switch4)
{
    if (switch4 != 0)
        switch_vector[3] = true;
    else switch_vector[3] = false;
}

QPixmap cam::Mat2Pixmap(Mat data)
{
    cvtColor(data, data, COLOR_RGBA2BGRA);
    return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
}

void cam::update()
{
    QVector<double> Graph_data;
    if (data_vector[4]*1000 > max_val_graph)
    {
        ui->customPlot->yAxis->setRange(0, data_vector[4]*1000);
        max_val_graph = data_vector[4]*1000;
    }
    Graph_data << 1000*data_vector[0] << 1000*data_vector[1] << 1000*data_vector[2] << 1000*data_vector[3] << 1000*data_vector[4];
    graph->setData(ticks, Graph_data);
    ui->customPlot->replot();
}

void cam::realtimeDataSlot()
{
    static QTime time(QTime::currentTime());
    static double constanta = time.minute() * 60 + time.hour() * 3600;
    double key = (time.elapsed() / 1000.0) + constanta;
    static double lastPointKey = 0;

    // Проверка масштабирования графика: понижение максимума на оси у
    if (flag == 0)
    {
        if ( ((1 / data_vector[4]) * 2) < max_val_graph2)
        {
            max_val_graph2 = 32;
            ui->customPlot2->yAxis->setRange(0, max_val_graph2);
            flag = 1;
        }
    }
    // Повышение максимума на оси у
    else if ( (1 / data_vector[4]) > max_val_graph2)
    {
        max_val_graph2 = 250;
        ui->customPlot2->yAxis->setRange(0, max_val_graph2);
        flag = 0;
    }

    ui->customPlot2->graph(0)->addData(key, 1 / data_vector[4]);
    lastPointKey = key;

    ui->customPlot2->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot2->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
              QString("%1 FPS, Total Data points: %2")
              .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
              .arg(ui->customPlot2->graph(0)->data()->size())
              , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}
