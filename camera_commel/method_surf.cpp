#include "method_surf.h"

method_surf::method_surf()
{
    example = imread ("/home/camel212/QtProjects/detail_3.png", CV_LOAD_IMAGE_GRAYSCALE);
    if (!example.data)
        qDebug() << "Image didn't opened!";
    blur(example,example, Size(1, 1));

    surf = SURF::create(minHess, 3, 2, false, false);
    surf->detectAndCompute(example, Mat(), kp_example, des_example, false);

    example_corners.resize(4);
    example_corners[0] = cvPoint(0,0);
    example_corners[1] = cvPoint( example.cols, 0 );
    example_corners[2] = cvPoint( example.cols, example.rows );
    example_corners[3] = cvPoint( 0, example.rows );
}

void method_surf::Method_surf_ON(Mat mat_vector, Mat &mat_vector_out)
{
    mat_vector_out = mat_vector;

    obj.clear();
    scene.clear();
    kp_image.clear();
    des_image.release();
    H.release();
    matches.clear();
    goodMatches.clear();

    vector<Point2f> scene_corners;

    cvtColor(mat_vector, camera_image, COLOR_BGR2GRAY);
    surf->detectAndCompute(camera_image, Mat(), kp_image,  des_image);
    qDebug() << kp_image.size();

    if ( ! ((des_image.empty()) || (des_example.empty())) )
    {
        if(des_example.type()!=CV_32F)
            des_example.convertTo(des_example, CV_32F);
        if(des_image.type()!=CV_32F)
            des_image.convertTo(des_image, CV_32F);

        matcher.match(des_example, des_image, matches);
        for(int i = 0; i < des_example.rows; i++)
        {
            distance = matches[i].distance;
            if(distance < min)
                min = distance;
        }
        for( int i = 0; i < des_example.rows; i++ )
        {
            if( matches[i].distance <  3*min)
                goodMatches.push_back( matches[i] );
        }
        for( size_t i = 0; i < goodMatches.size(); i++ )
        {
            obj.push_back( kp_example[ goodMatches[ i ].queryIdx ].pt );
            scene.push_back( kp_image[ goodMatches[ i ].trainIdx ].pt );
        }
        try {
           H = findHomography( obj, scene, CV_RANSAC );
           if ( !H.empty())
               perspectiveTransform( example_corners, scene_corners, H );

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

bool method_surf::check(const vector<Point2f> scene_corners)
{
    if(scene_corners.size() != 4)
        return false;
    else {
        auto diag1 = sqrt( pow(scene_corners[0].x - scene_corners[2].x,2) + pow(scene_corners[0].y - scene_corners[2].y,2));
        auto diag2 = sqrt( pow(scene_corners[1].x - scene_corners[3].x,2) + pow(scene_corners[1].y - scene_corners[3].y,2));
        if ( (diag1 < 280) || (diag2 < 280)  || (diag1 > 1200) || (diag2 > 1200) ||
             (scene_corners[0].x > 1032) ||  (scene_corners[0].x < 0) || (scene_corners[0].y > 772) || (scene_corners[0].y < 0) ||
             (scene_corners[1].x > 1032) ||  (scene_corners[1].x < 0) || (scene_corners[1].y > 772) || (scene_corners[1].y < 0) ||
             (scene_corners[2].x > 1032) ||  (scene_corners[2].x < 0) || (scene_corners[2].y > 772) || (scene_corners[2].y < 0) ||
             (scene_corners[3].x > 1032) ||  (scene_corners[3].x < 0) || (scene_corners[3].y > 772) || (scene_corners[3].y < 0))
            return false;
        return true;
    }
}
