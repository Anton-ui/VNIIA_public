#include "method_xml.h"

method_xml::method_xml()
{
    face_cascade.load("/home/trk/QtProjects/VNIIA_public/haarcascade_frontalface_alt.xml");
    scene_corners.resize(4);
}

void method_xml::Method_xml_ON(Mat mat_vector, Mat &mat_vector_out)
{
    face_cascade.detectMultiScale(mat_vector, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
    for (v = 0; v < faces.size(); ++v)
    {
        scene_corners[0] = Point(faces[v].x,                  faces[v].y);
        scene_corners[1] = Point(faces[v].x,                  faces[v].y + faces[v].height);
        scene_corners[2] = Point(faces[v].x + faces[v].width, faces[v].y + faces[v].height);
        scene_corners[3] = Point(faces[v].x + faces[v].width, faces[v].y);

        line(mat_vector_out, scene_corners[ 0 ] , scene_corners[ 1 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 1 ] , scene_corners[ 2 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 2 ] , scene_corners[ 3 ] , Scalar( 255, 0, 0 ), 4);
        line(mat_vector_out, scene_corners[ 3 ] , scene_corners[ 0 ] , Scalar( 255, 0, 0 ), 4);
    }
}
