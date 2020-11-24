#ifndef METHOD_TEMPLATE_H
#define METHOD_TEMPLATE_H

#include "Sources/cameraclass.h"

class method_template
{
public:
    method_template();
    void Method_temp_ON(Mat mat_vector, Mat &mat_vector_out);

private:
    QVector<Point2f> scene_corners;
    Mat templat;
    Mat result;
    Mat gray;
    Mat gray2;
    int result_cols;
    int result_rows;
    double null = 0;
    Point minLoc;
    Point maxLoc;
};

#endif // METHOD_TEMPLATE_H
