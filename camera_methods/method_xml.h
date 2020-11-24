#ifndef METHOD_XML_H
#define METHOD_XML_H

#include "Sources/cameraclass.h"

using namespace std;

class method_xml
{
public:
    method_xml();
    void Method_xml_ON(Mat mat_vector, Mat &mat_vector_out);

private:
    QVector<Point2f> scene_corners;
    vector<Rect> faces;
    CascadeClassifier face_cascade;
    unsigned long v;
};

#endif // METHOD_XML_H
