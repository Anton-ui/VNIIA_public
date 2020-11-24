#ifndef GLOBAL_PARAMS_H
#define GLOBAL_PARAMS_H

#include "cameraclass.h"

QVector<XimeaCamera>       cam_vector;
QVector<QGraphicsView*>   view_vector;
QVector<QPixmap>        pixmap_vector;
QVector<Mat>               mat_vector;
QVector<bool>           switch_vector;
QVector<QGraphicsScene*> scene_vector;

QPen pen(Qt::blue, 5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);


#endif // GLOBAL_PARAMS_H
