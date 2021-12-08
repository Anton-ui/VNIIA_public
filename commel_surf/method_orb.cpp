#include "method_orb.h"

method_orb::method_orb()
{   
    example = imread ("/home/trk/QtProjects/VNIIA_public/cup.png", CASCADE_SCALE_IMAGE);
    if (!example.data)
        qDebug() << "Image didn't opened!";
    blur(example,example, Size(1, 1));

    detector = ORB::create();
    detector->detect(example, keypoints_object);
    extractor = ORB::create();
    extractor->compute(example, keypoints_object, descriptors_object );

    example_corners.resize(4);
    example_corners[0] = Point(0,0);
    example_corners[1] = Point( example.cols, 0 );
    example_corners[2] = Point( example.cols, example.rows );
    example_corners[3] = Point( 0, example.rows );
}

void method_orb::Method_orb_ON(Mat mat_vector, Mat &mat_vector_out)
{
    mat_vector_out = mat_vector;
    descriptors_scene.release();
    keypoints_scene.clear();
    obj.clear();
    scene.clear();
    H.release();
    matches.clear();
    good_matches.clear();

    // find the keypoints and descriptors with ORB
    detector->detect(gray_image2, keypoints_scene);
    extractor->compute(gray_image2, keypoints_scene, descriptors_scene );

    if ( ! ((descriptors_scene.empty()) || (descriptors_object.empty())) )
    {
        if(descriptors_object.type()!=CV_32F)
            descriptors_scene.convertTo(descriptors_scene, CV_32F);
        if(descriptors_scene.type()!=CV_32F)
            descriptors_object.convertTo(descriptors_object, CV_32F);

        matcher.match( descriptors_object, descriptors_scene, matches );
        double max_dist = 0;
        double min_dist = 100;

        //-- Quick calculation of max and min distances between keypoints
        for( int i = 0; i < descriptors_object.rows; i++ )
        {
            double dist = matches[i].distance;
            if( dist < min_dist ) min_dist = dist;
            if( dist > max_dist ) max_dist = dist;
        }
        for( int i = 0; i < descriptors_object.rows; i++ )
        {
            if( matches[i].distance < 3*min_dist )
                good_matches.push_back( matches[i]);
        }
        for( int i = 0; i < good_matches.size(); i++ )
        {
            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }
        try {
            H = findHomography( obj, scene, RANSAC );
            if ( !H.empty())
            {
                vector<Point2f> scene_corners;
                perspectiveTransform( example_corners, scene_corners, H );
                if(check(scene_corners))
                {
                    line(mat_vector_out, scene_corners[ 0 ] , scene_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
                    line(mat_vector_out, scene_corners[ 1 ] , scene_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
                    line(mat_vector_out, scene_corners[ 2 ] , scene_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
                    line(mat_vector_out, scene_corners[ 3 ] , scene_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
                }
            }
        }
        catch(Exception e) {}
    }
}

bool method_orb::check(const vector<Point2f> scene_corners)
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
