#ifndef METHOD_AKAZE_H
#define METHOD_AKAZE_H

#include "cameraclass.h"

using namespace std;

class method_akaze
{
public:
    method_akaze();
    void Method_akaze_ON(Mat mat_vector, Mat &mat_vector_out);

private:
    bool check(const vector<Point2f> scene_corners);
    Ptr<AKAZE> akaze;
    vector<KeyPoint> kp;
    vector<KeyPoint> kp_query;
    vector<DMatch> matches;
    vector<DMatch> good_matches;
    vector<Point2f> obj, scene;
    vector<Point2f> obj_corners;
    double min = 100;
    double distance;
    Mat H;
    Mat image;
    Mat descr;
    Mat descr_query;
    Mat example;
};

#endif // METHOD_AKAZE_H
