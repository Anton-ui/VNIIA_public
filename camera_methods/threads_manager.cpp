#include "threads_manager.h"

threads_manager::threads_manager(XimeaCamera *cam, QObject *parent):QObject(parent)
{
    cam_ptr.reset(cam);
}

void threads_manager::UpdateInfo(QVector<bool> in_switch_vector)
{
    switch_vector = in_switch_vector;
}


//void threads_manager::process(int in_var, Mat &mat_vector, QPixmap &pixmap_vector, QGraphicsView &view_vector, QGraphicsScene &scene_vector,
//                                QPen pen)
void threads_manager::process()
{
    pixmap.loadFromData(cam_ptr->GetUcharImage());
    mat = cam_ptr->GetMatImage();


    //emit finished(cam_ptr->cam_id, pixmap, vector(Point2f))

    /*if(in_var==v)
    {
        qDebug() << "here" << in_var;

    view_vector.scene()->clear();
    view_vector.scene()->addPixmap(pixmap_vector);

    if (switch_vector[0])
        temp.Method_temp_ON(mat_vector, scene_vector, pen);
    if (switch_vector[1])
        xml.Method_xml_ON(mat_vector, scene_vector, pen);
    if (switch_vector[2])
        surf.Method_surf_ON(mat_vector, scene_vector, pen);
    if (switch_vector[3])
        akaze.Method_akaze_ON(mat_vector, scene_vector, pen);

    view_vector.setScene(&scene_vector);
    view_vector.fitInView(pixmap_vector.rect(), Qt::KeepAspectRatio);

    }
    */
}
