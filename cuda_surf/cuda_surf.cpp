#include "cuda_surf.h"

cuda_surf::cuda_surf()
{
    example = imread ("/home/jetson/QtProjects/cup.png", CV_LOAD_IMAGE_GRAYSCALE);
    if (!example.data)
        qDebug() << "Image didn't opened!";
    blur(example,example, Size(1, 1));

    gpu_example.upload(example);
    surf(gpu_example, GpuMat(), keypoints1GPU, descriptors1GPU);
    //surf()(gpu_example, GpuMat(), kp_example, des_example);

    example_corners.resize(4);
    example_corners[0] = cvPoint(0,0);
    example_corners[1] = cvPoint( example.cols, 0 );
    example_corners[2] = cvPoint( example.cols, example.rows );
    example_corners[3] = cvPoint( 0, example.rows );
}

void cuda_surf::Detect_cuda_surf(cuda::GpuMat grey_mat_vector, cuda::GpuMat &mat_vector_out)
{
    mat_vector_out = grey_mat_vector;

    obj.clear();
    scene.clear();
    keypoints1.clear();
    image_corners.clear();
    matches.clear();
    goodMatches.clear();
    descriptors1.clear();

    keypoints2GPU.release();
    descriptors2GPU.release();
    H.release();

    surf(grey_mat_vector, GpuMat(), keypoints2GPU, descriptors2GPU);
    matcher = cv::cuda::DescriptorMatcher::createBFMatcher(surf.defaultNorm());
    matcher->match(descriptors1GPU, descriptors2GPU, matches);

    surf.downloadKeypoints(keypoints1GPU, keypoints1);
    surf.downloadKeypoints(keypoints2GPU, keypoints2);
    surf.downloadDescriptors(descriptors1GPU, descriptors1);
    surf.downloadDescriptors(descriptors2GPU, descriptors2);

    //Mat img_matches;
    //drawMatches(Mat(img1), keypoints1, Mat(img2), keypoints2, matches, img_matches);

    //if ( ! ((des_image.empty()) || (des_example.empty())) )
    //{
        matcher->match(descriptors1GPU, descriptors2GPU, matches);

        surf.downloadKeypoints(keypoints1GPU, keypoints1);
        surf.downloadKeypoints(keypoints2GPU, keypoints2);
        surf.downloadDescriptors(descriptors1GPU, descriptors1);
        surf.downloadDescriptors(descriptors2GPU, descriptors2);
        for(var = 0; var < descriptors1GPU.rows; var++)
        {
            distance = matches[var].distance;
            if(distance < min)
                min = distance;
        }
        for(var = 0; var < descriptors1GPU.rows; var++)
        {
            if( matches[var].distance <  3*min)
                goodMatches.push_back( matches[var] );
        }
        for( size_t i = 0; i < goodMatches.size(); i++ )
        {
            obj.push_back(  keypoints1[ goodMatches[ i ].queryIdx ].pt );
            scene.push_back(keypoints2[ goodMatches[ i ].trainIdx ].pt );
        }
        //void downloadKeypoints(const GpuMat& keypointsGPU, std::vector<KeyPoint>& keypoints);
        // проверка на наличие соответствия
        try {
           H = findHomography( obj, scene, CV_RANSAC );
           if ( !H.empty())
               perspectiveTransform( example_corners, image_corners, H );

           if(check(image_corners))
           {
               line(mat_vector_out, image_corners[ 0 ] , image_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 1 ] , image_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 2 ] , image_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 3 ] , image_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
           }
        }
        catch(Exception e)
        {}
    //}
    /*mat_vector_out = grey_mat_vector;

    obj.clear();
    scene.clear();
    kp_image.clear();
    image_corners.clear();
    matches.clear();
    goodMatches.clear();

    des_image.release();
    H.release();

    gpu_grey_image.upload(grey_mat_vector);
    surf()(gpu_grey_image, GpuMat(), kp_image, des_image);

    // проверка наличия изображений
    if ( ! ((des_image.empty()) || (des_example.empty())) )
    {
        // check type
        if(des_example.type()!=CV_32F)
            des_example.convertTo(des_example, CV_32F);
        if(des_image.type()!=CV_32F)
            des_image.convertTo(des_image, CV_32F);

        matcher.match(des_example, des_image, matches);
        for(var = 0; var < des_example.rows; var++)
        {
            distance = matches[var].distance;
            if(distance < min)
                min = distance;
        }
        for(var = 0; var < des_example.rows; var++)
        {
            if( matches[var].distance <  3*min)
                goodMatches.push_back( matches[var] );
        }
        for( size_t i = 0; i < goodMatches.size(); i++ )
        {
            obj.push_back( kp_example[ goodMatches[ i ].queryIdx ].pt );
            scene.push_back( kp_image[ goodMatches[ i ].trainIdx ].pt );
        }
        //void downloadKeypoints(const GpuMat& keypointsGPU, std::vector<KeyPoint>& keypoints);
        // проверка на наличие соответствия
        try {
           H = findHomography( obj, scene, CV_RANSAC );
           if ( !H.empty())
               perspectiveTransform( example_corners, image_corners, H );

           if(check(scene_corners))
           {
               line(mat_vector_out, image_corners[ 0 ] , image_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 1 ] , image_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 2 ] , image_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
               line(mat_vector_out, image_corners[ 3 ] , image_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
           }
        }
        catch(Exception e)
        {}
    }*/
}

bool cuda_surf::check(const vector<Point2f> image_corners)
{
    if(image_corners.size() != 4)
        return false;
    else {
        auto diag1 = sqrt( pow(image_corners[0].x - image_corners[2].x,2) + pow(image_corners[0].y - image_corners[2].y,2));
        auto diag2 = sqrt( pow(image_corners[1].x - image_corners[3].x,2) + pow(image_corners[1].y - image_corners[3].y,2));
        if ( (diag1 < 280) || (diag2 < 280)  || (diag1 > 1200) || (diag2 > 1200) ||
             (image_corners[0].x > 1032) ||  (image_corners[0].x < 0) || (image_corners[0].y > 772) || (image_corners[0].y < 0) ||
             (image_corners[1].x > 1032) ||  (image_corners[1].x < 0) || (image_corners[1].y > 772) || (image_corners[1].y < 0) ||
             (image_corners[2].x > 1032) ||  (image_corners[2].x < 0) || (image_corners[2].y > 772) || (image_corners[2].y < 0) ||
             (image_corners[3].x > 1032) ||  (image_corners[3].x < 0) || (image_corners[3].y > 772) || (image_corners[3].y < 0))
            return false;
        return true;
    }
}
