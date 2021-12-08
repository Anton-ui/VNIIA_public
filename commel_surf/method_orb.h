#ifndef METHOD_ORB_H
#define METHOD_ORB_H

#include "Sources/cameraclass.h"

using namespace std;

class method_orb
{
public:
    method_orb();
    void Method_orb_ON(Mat mat_vector, Mat &mat_vector_out);

private:
    bool check(const vector<Point2f> scene_corners);
    Mat example;
    Mat gray_image2;
    Mat descriptors_object, descriptors_scene;
    Ptr<FeatureDetector> detector;
    Ptr<DescriptorExtractor> extractor;
    Mat H;

    vector<KeyPoint> keypoints_object;
    vector<KeyPoint> keypoints_scene;
    vector< Point2f > obj;
    vector< Point2f > scene;

    vector<Point2f> example_corners;
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    vector< DMatch > good_matches;
};

#endif // METHOD_ORB_H
