#include "method_template.h"

method_template::method_template()
{
    templat = imread("/home/trk/QtProjects/VNIIA_public/cup.png", IMREAD_COLOR );
    if(templat.empty())
      qDebug() << "Can't read one of the images";
    cvtColor(templat, gray2, COLOR_BGR2GRAY);
    scene_corners.resize(4);
}

void method_template::Method_temp_ON(Mat mat_vector, Mat &mat_vector_out)
{
    result_cols = mat_vector.cols - templat.cols + 1;
    result_rows = mat_vector.rows - templat.rows + 1;
    result.create( result_rows, result_cols, CV_32F);

    cvtColor(mat_vector, gray, COLOR_BGR2GRAY);
    matchTemplate( gray, gray2, result, 5);
    normalize( result, result);
    double max =0;

    minMaxLoc( result, &null, &max, &minLoc, &maxLoc, Mat() );
    if(max > 0.006)
    {
        scene_corners[0] = Point(maxLoc.x,                maxLoc.y);
        scene_corners[1] = Point(maxLoc.x,                maxLoc.y + templat.rows);
        scene_corners[2] = Point(maxLoc.x + templat.cols, maxLoc.y + templat.rows);
        scene_corners[3] = Point(maxLoc.x + templat.cols, maxLoc.y);

        line(mat_vector_out, scene_corners[ 0 ] , scene_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 1 ] , scene_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 2 ] , scene_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 3 ] , scene_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
    }
}
