#ifndef THREADS_MANAGER_H
#define THREADS_MANAGER_H

#include "method_xml.h"
#include "method_surf.h"
#include "method_template.h"
#include "method_akaze.h"

using namespace cv;

class threads_manager : public QObject
{
    Q_OBJECT

public:
    threads_manager(XimeaCamera *cam, QObject *parent = nullptr);
    void UpdateInfo(QVector<bool> in_switch_vector);

public slots:
    //void process(int in_var, Mat &mat_vector, QPixmap &pixmap_vector, QGraphicsView &view_vector, QGraphicsScene &scene_vector,
    //              QPen pen);
    void process();

signals:
    //void finished(int cam_id, QPixmap data, );

private:
    std::unique_ptr<XimeaCamera> cam_ptr;
    method_xml       xml;
    method_surf     surf;
    method_template temp;
    method_akaze   akaze;
    QPixmap pixmap;
    cv::Mat mat;
    QVector<bool> switch_vector;
};

#endif // THREADS_MANAGER_H
