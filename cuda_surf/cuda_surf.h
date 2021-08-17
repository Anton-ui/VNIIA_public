#ifndef CUDA_SURF_H
#define CUDA_SURF_H

#include "Sources/cameraclass.h"
#include "opencv2/opencv_modules.hpp"

#include "opencv2/cudafeatures2d.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/core/cuda.hpp"

#include <opencv2/xfeatures2d/cuda.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv::cuda;

class cuda_surf
{
public:
    cuda_surf();
    void Detect_cuda_surf(cuda::GpuMat mat_vector, cuda::GpuMat &mat_vector_out);

private:
    bool check(const vector<Point2f> scene_corners);

    //Ptr<cuda::SURF_CUDA> surf();
    //cuda::DescriptorMatcher* matcher;
    int var = 0;

    cv::cuda::SURF_CUDA surf;
    GpuMat keypoints1GPU, keypoints2GPU;
    GpuMat descriptors1GPU, descriptors2GPU;

    Mat example;
    cuda::GpuMat gpu_example;

    Ptr<cv::cuda::DescriptorMatcher> matcher;
    vector<KeyPoint> keypoints1, keypoints2;
    vector<float> descriptors1, descriptors2;

    vector<DMatch> matches;
    vector<DMatch> goodMatches;
    vector< Point2f > obj;
    vector< Point2f > scene;

    float distance;
    float min=100;

    vector<Point2f> example_corners;
    vector<Point2f> image_corners;
    Mat H;
    /*// example
    Mat example;
    cuda::GpuMat gpu_example;
    vector<KeyPoint> kp_example;
    cuda::GpuMat des_example;
    vector<Point2f> example_corners;

    // image
    cuda::GpuMat gpu_grey_image;
    vector<KeyPoint> kp_image;
    cuda::GpuMat des_image;
    vector<Point2f> image_corners;

    cuda::GpuMat H;

    vector<DMatch> matches;
    vector<DMatch> goodMatches;
    vector< Point2f > obj;
    vector< Point2f > scene;

    float distance;
    float min=100;*/
};

#endif // CUDA_SURF_H
