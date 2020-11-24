#include "stereo.h"
#include "ui_stereo.h"

///////////// ВНИМАНИЕ! ////////////////
/// этот говно - хардкод работает только для 2 камер Ximea ///
///

stereo::stereo(QWidget *parent) :
    QMainWindow(parent),
    patternsize(Size(8, 5)),
    //patternsize(Size(11, 4)),
    square_size(1.0f),
    ui(new Ui::stereo)
{
    number = cam_manager.GetNumberOfConnectedCameras();
    cam_vector.resize(number);
    mat_vector.resize(number);
    grey_mat_vector.resize(number);
    undistort_vector.resize(number);

    pixmap_vector.resize(number + 1);

    ui->setupUi(this);
    timer.reset(new QTimer);
    timer->setInterval(1000/4);
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(CaptureImage()));

    for (int var = 0; var < number; ++var)
    {
        view_vector.append(new QGraphicsView);
        scene_vector.append(new QGraphicsScene(this));
        ui->Layout->addWidget(view_vector[var]);
        cam_vector[var].InitCamera(var, number);
        view_vector[var]->setScene(scene_vector[var]);
    }

    // окно для стерео
    view_vector.append(new QGraphicsView);
    scene_vector.append(new QGraphicsScene(this));
    ui->Layout->addWidget(view_vector[2]);
    view_vector[2]->setScene(scene_vector[2]);

    // настройка стерео
    sbm = StereoBM::create(0, 21);


    /*sbm->setNumDisparities(128);
    sbm->setMinDisparity(4);
    sbm->setSpeckleWindowSize(45);
    sbm->setSpeckleRange(16);*/

    sbm->setNumDisparities(112);
    sbm->setPreFilterSize(5);
    sbm->setPreFilterCap(61);
    sbm->setMinDisparity(-39);
    sbm->setTextureThreshold(507);
    sbm->setUniquenessRatio(0);
    sbm->setSpeckleWindowSize(0);
    sbm->setSpeckleRange(8);
    sbm->setDisp12MaxDiff(1);
}

stereo::~stereo()
{
    delete ui;
    timer.release();
}

void stereo::on_Start_clicked()
{
    timer->start();
    for (int var = 0; var < number; ++var)
        if(cam_vector[var].Status() != XI_ON)
            cam_vector[var].Start();
        else qDebug() << "Camera" << var << "not started";
}

void stereo::on_Stop_clicked()
{
    for (int var = 0; var < number; ++var)
        cam_vector[var].Stop();
    timer->stop();
}

void stereo::PerformCalibration()
{
    if(calib_counter < 20)
    {
        vector< Point2f > corners1, corners2;
        vector< Point3f > obj;
        bool found1 = false, found2 = false;

        found1 = findChessboardCorners(mat_vector[0], patternsize, corners1,  CV_CALIB_CB_FAST_CHECK  );
        found2 = findChessboardCorners(mat_vector[1], patternsize, corners2,   CV_CALIB_CB_FAST_CHECK  );
        //found1 = findCirclesGrid2(mat_vector[0], patternsize, corners1, CALIB_CB_ASYMMETRIC_GRID | CALIB_CB_FAST_CHECK);
        //found2 = findCirclesGrid2(mat_vector[1], patternsize, corners2, CALIB_CB_ASYMMETRIC_GRID | CALIB_CB_FAST_CHECK);

        qDebug() << "status found1/2: " << found1 << found2;
        if (found1)
        {
            cornerSubPix(grey_mat_vector[0], corners1, Size(5, 5), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(grey_mat_vector[0], patternsize, corners1, found1);

            pixmap_vector[0] = Mat2Pixmap(grey_mat_vector[0], true);
            view_vector[0]->scene()->clear();
            view_vector[0]->scene()->addPixmap(pixmap_vector[0]);
            view_vector[0]->setScene(scene_vector[0]);
            view_vector[0]->fitInView(pixmap_vector[0].rect(), Qt::KeepAspectRatio);
        }
        if (found2)
        {
            cornerSubPix(grey_mat_vector[1], corners2, Size(5, 5), Size(-1, -1),TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(grey_mat_vector[1], patternsize, corners2, found2);

            pixmap_vector[1] = Mat2Pixmap(grey_mat_vector[1], true);
            view_vector[1]->scene()->clear();
            view_vector[1]->scene()->addPixmap(pixmap_vector[1]);
            view_vector[1]->setScene(scene_vector[1]);
            view_vector[1]->fitInView(pixmap_vector[1].rect(), Qt::KeepAspectRatio);
        }
        if (found1 && found2)
        {
            for (int y = 0; y < patternsize.height; ++y)
                for (int x = 0; x < patternsize.width; ++x)
                    obj.push_back(Point3f(y * square_size, x * square_size, 0));
            /*for (int i = 0; i < patternsize.width * patternsize.height; i++) {
                 obj.push_back(Point3f(i / patternsize.height, i % patternsize.width, 0.0f));
                 }*/
            imagePoints1.push_back(corners1);
            imagePoints2.push_back(corners2);
            object_points.push_back(obj);
            ++calib_counter;
        }
    }
    else
    {
        qDebug() << "Calibration started";
        // calibration
        qDebug() << stereoCalibrate(object_points, imagePoints1, imagePoints2, CM1, D1, CM2, D2, mat_vector[0].size(), R, T, E, F,
                                                CV_CALIB_SAME_FOCAL_LENGTH ,
                                                cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 1e-6) );

        // rectification
        stereoRectify(CM1, D1, CM2, D2, mat_vector[0].size(), R, T, R1, R2, P1, P2, Q);

        // Starting to apply undistort
        map1x = Mat(mat_vector[0].size().height, mat_vector[0].size().width, CV_32F);
        map1y = Mat(mat_vector[0].size().height, mat_vector[0].size().width, CV_32F);
        map2x = Mat(mat_vector[1].size().height, mat_vector[1].size().width, CV_32F);
        map2y = Mat(mat_vector[1].size().height, mat_vector[1].size().width, CV_32F);
        initUndistortRectifyMap(CM1, D1, R1, P1, mat_vector[0].size(), CV_32FC1, map1x, map1y);
        initUndistortRectifyMap(CM2, D2, R2, P2, mat_vector[1].size(), CV_32FC1, map2x, map2y);

        // The rectified images
        undistort_vector[0] = Mat(mat_vector[0].size(), mat_vector[0].type());
        undistort_vector[1] = Mat(mat_vector[1].size(), mat_vector[1].type());

        // Show undistort
        remap(mat_vector[0], undistort_vector[0], map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
        remap(mat_vector[1], undistort_vector[1], map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

        qDebug() << "Calibration successful";
        calibration = false;
        done = true;
        }
    }



void stereo::on_Calibration_clicked()
{
    calib_counter = 0;
    calibration = !calibration;

    //https://github.com/sourishg/stereo-calibration/blob/master/calib_stereo.cpp
    //https://web-answers.ru/c/opencv-camera-stereo-calibration-strannye.html
    //https://sourishghosh.com/2016/stereo-calibration-cpp-opencv/

    //https://docs.opencv.org/3.4/db/d8a/classcv_1_1cuda_1_1StereoBM.html
}

void stereo::CaptureImage()
{
    if((cam_vector[0].Status() == XI_ON) && (cam_vector[1].Status() == XI_ON))
    {
        for (int var = 0; var < number + 1; var++)
        {
            if (var == 2)
            {
                /*if (done)
                {
                    remap(mat_vector[0], mat_vector[0], map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
                    remap(mat_vector[1], mat_vector[1], map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
                }*/
                cvtColor(mat_vector[0],grey_mat_vector[0], COLOR_BGR2GRAY);
                cvtColor(mat_vector[1],grey_mat_vector[1], COLOR_BGR2GRAY);
                sbm->compute(grey_mat_vector[0], grey_mat_vector[1], disp);
                normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
                pixmap_vector[var] = Mat2Pixmap(disp8, false);
            }
            else
            {
                //pixmap_vector[var].loadFromData(cam_vector[var].GetUcharImage());
                mat_vector[var] = cam_vector[var].GetMatImage();
                if (done)
                {
                    if (var == 0)
                    remap(mat_vector[var], mat_vector[var], map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
                    if (var == 1)
                    remap(mat_vector[var], mat_vector[var], map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

                }
                pixmap_vector[var] = Mat2Pixmap(mat_vector[var], true);
            }
            view_vector[var]->scene()->clear();
            view_vector[var]->scene()->addPixmap(pixmap_vector[var]);

            view_vector[var]->setScene(scene_vector[var]);
            view_vector[var]->fitInView(pixmap_vector[var].rect(), Qt::KeepAspectRatio);
        }
        if(calibration)
            PerformCalibration();
    }
}

QPixmap stereo::Mat2Pixmap(Mat data, bool isUsualMat)
{

    if (isUsualMat)
    {
        if(data.type() != 0)
        {
            cvtColor(data, data, COLOR_RGBA2BGRA);
            return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_RGBX8888));
        }
        else return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_Indexed8));
    }
    else return QPixmap::fromImage(QImage((unsigned char*) data.data, data.cols, data.rows, QImage:: Format_Indexed8));
}

void stereo::on_Save_clicked()
{
    FileStorage fs("stereocalib.yml", FileStorage::WRITE);
    fs << "CM1" << CM1;
    fs << "CM2" << CM2;
    fs << "D1" << D1;
    fs << "D2" << D2;
    fs << "R" << R;
    fs << "T" << T;
    fs << "E" << E;
    fs << "F" << F;
    fs << "R1" << R1;
    fs << "R2" << R2;
    fs << "P1" << P1;
    fs << "P2" << P2;
    fs << "Q" << Q;
    fs.release();
    qDebug() << "Save is successful";
}

void stereo::on_Load_clicked()
{
    FileStorage fs("stereocalib.yml", FileStorage::READ);
    fs["CM1"] >> CM1;
    fs["CM2"] >> CM2;
    fs["D1"] >> D1;
    fs["D2"] >> D2;
    fs["R"] >> R;
    fs["T"] >> T;
    fs["E"] >> E;
    fs["F"] >> F;
    fs["R1"] >> R1;
    fs["R2"] >> R2;
    fs["P1"] >> P1;
    fs["P2"] >> P2;
    fs["Q"] >> Q;
    fs.release();

    // Starting to apply undistort
    map1x = Mat(mat_vector[0].size().height, mat_vector[0].size().width, CV_32F);
    map1y = Mat(mat_vector[0].size().height, mat_vector[0].size().width, CV_32F);
    map2x = Mat(mat_vector[1].size().height, mat_vector[1].size().width, CV_32F);
    map2y = Mat(mat_vector[1].size().height, mat_vector[1].size().width, CV_32F);
    initUndistortRectifyMap(CM1, D1, R1, P1, mat_vector[0].size(), CV_32FC1, map1x, map1y);
    initUndistortRectifyMap(CM2, D2, R2, P2, mat_vector[1].size(), CV_32FC1, map2x, map2y);

    // The rectified images
    undistort_vector[0] = Mat(mat_vector[0].size(), mat_vector[0].type());
    undistort_vector[1] = Mat(mat_vector[1].size(), mat_vector[1].type());

    // Show undistort
    remap(mat_vector[0], undistort_vector[0], map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    remap(mat_vector[1], undistort_vector[1], map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    done = true;
    qDebug() << "Load is successful";
}

void stereo::on_horizontalSlider_sliderMoved(int position)
{
    sbm->setNumDisparities(position*16);
}

void stereo::on_horizontalSlider_2_sliderMoved(int position)
{
    sbm->setPreFilterSize((position*2)+1);
}

void stereo::on_horizontalSlider_3_sliderMoved(int position)
{
    sbm->setPreFilterCap(position);
}

void stereo::on_horizontalSlider_4_sliderMoved(int position)
{
    sbm->setMinDisparity(position);
}

void stereo::on_horizontalSlider_5_sliderMoved(int position)
{
    sbm->setTextureThreshold(position);
}

void stereo::on_horizontalSlider_6_sliderMoved(int position)
{
    sbm->setUniquenessRatio(position);
}

void stereo::on_horizontalSlider_7_sliderMoved(int position)
{
    sbm->setSpeckleWindowSize(position);
}

void stereo::on_horizontalSlider_8_sliderMoved(int position)
{
    sbm->setSpeckleRange(position);
}

void stereo::on_horizontalSlider_9_sliderMoved(int position)
{
    sbm->setDisp12MaxDiff(position);
}
