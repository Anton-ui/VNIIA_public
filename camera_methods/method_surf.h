#ifndef METHOD_SURF_H
#define METHOD_SURF_H

#include "Sources/cameraclass.h"

using namespace std;
using namespace cv::xfeatures2d;

class method_surf
{
public:
    method_surf();
    void Method_surf_ON(Mat mat_vector, Mat &mat_vector_out);

private:
    bool check(const vector<Point2f> scene_corners);
    FlannBasedMatcher matcher;
    Ptr<SURF> surf;
    QPainter paint;

    Mat example;
    Mat camera_image;
    Mat des_example, des_image, H;

    vector<KeyPoint> kp_example;
    vector<KeyPoint> kp_image;
    vector<DMatch> matches;
    vector<DMatch> goodMatches;
    vector< Point2f > obj;
    vector< Point2f > scene;
    vector<Point2f> example_corners;

    int minHess=500;
    float distance;
    float min=100;
};

#endif // METHOD_SURF_H
