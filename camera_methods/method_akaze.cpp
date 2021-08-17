#include "method_akaze.h"

BFMatcher matcher(NORM_HAMMING);

method_akaze::method_akaze()
{
    example = imread ("/home/camel212/QtProjects/detail_3.png", CV_LOAD_IMAGE_GRAYSCALE);
    if (!example.data)
        qDebug() << "Image didn't opened!";
    blur(example,example, Size(1, 1));

    akaze = AKAZE::create();
    akaze->detectAndCompute(example, Mat(), kp, descr, false);

    obj_corners.resize(4);
    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( example.cols, 0 );
    obj_corners[2] = cvPoint( example.cols, example.rows );
    obj_corners[3] = cvPoint( 0, example.rows );
}

void method_akaze::Method_akaze_ON(Mat mat_vector, Mat &mat_vector_out)
{
    mat_vector_out = mat_vector;

    obj.clear();
    scene.clear();
    kp_query.clear();
    descr_query.release();
    H.release();
    matches.clear();
    good_matches.clear();

    vector<Point2f> scene_corners;

    cvtColor(mat_vector, image, COLOR_BGR2GRAY);
    akaze->detectAndCompute(image, Mat(), kp_query,  descr_query);

    if ( ! ((descr.empty()) || (descr_query.empty())) )
    {
        matcher.match(descr, descr_query, matches);
        for(int i = 0; i < descr.rows; i++)
        {
            distance = matches[i].distance;
            if(distance < min)
                min = distance;
        }
        for( int i = 0; i < descr.rows; i++ )
        {
            if( matches[i].distance <  3*min)
                good_matches.push_back( matches[i] );
        }
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
                obj.push_back( kp[ good_matches[ i ].queryIdx ].pt );
                scene.push_back( kp_query[ good_matches[ i ].trainIdx ].pt );
        }
        try {
           Mat H = findHomography( obj, scene, CV_RANSAC );
           if (! H.empty())
               perspectiveTransform( obj_corners, scene_corners, H );

           if(check(scene_corners))
           {
               line(mat_vector_out, scene_corners[ 0 ] , scene_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, scene_corners[ 1 ] , scene_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, scene_corners[ 2 ] , scene_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, scene_corners[ 3 ] , scene_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
           }
        }
        catch(Exception e)
          {}
    }
}

bool method_akaze::check(const vector<Point2f> scene_corners)
{
    if(scene_corners.size() != 4)
        return false;
    else
    {
        auto diag1 = sqrt( pow(scene_corners[0].x - scene_corners[2].x,2) + pow(scene_corners[0].y - scene_corners[2].y,2));
        auto diag2 = sqrt( pow(scene_corners[1].x - scene_corners[3].x,2) + pow(scene_corners[1].y - scene_corners[3].y,2));
        if ( (diag1 < 250) || (diag2 < 250)  || (diag1 > 1200) || (diag2 > 1200) ||
             (scene_corners[0].x > 1032) ||  (scene_corners[0].x < 0) || (scene_corners[0].y > 772) || (scene_corners[0].y < 0) ||
             (scene_corners[1].x > 1032) ||  (scene_corners[1].x < 0) || (scene_corners[1].y > 772) || (scene_corners[1].y < 0) ||
             (scene_corners[2].x > 1032) ||  (scene_corners[2].x < 0) || (scene_corners[2].y > 772) || (scene_corners[2].y < 0) ||
             (scene_corners[3].x > 1032) ||  (scene_corners[3].x < 0) || (scene_corners[3].y > 772) || (scene_corners[3].y < 0))
            return false;
        return true;
    }
}
