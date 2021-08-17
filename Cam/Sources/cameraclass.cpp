#include "cameraclass.h"

XimeaCamera::XimeaCamera()
{

}

int XimeaCamera::InitCamera(unsigned long in_cam_id, unsigned long in_cam_num)
{

    cam_id = in_cam_id;
    cam_num = in_cam_num;

    /////////////////////////////////////////////////////////////////
    /// Формирование правильного битрейта для того, чтобы камеры не конфликтовали
    /////////////////////////////////////////////////////////////////
    xiSetParamInt(nullptr, XI_PRM_AUTO_BANDWIDTH_CALCULATION, XI_OFF);
    cam.OpenByID(cam_id);
    int model_id = cam.GetModelId();
    //qDebug() << "Model ID:" << model_id;
    /////////////////////////////////////////////////////////////////
    /// Цветное изображение, автоматический баланс белого
    /////////////////////////////////////////////////////////////////
    cam.SetImageDataFormat(XI_RGB24);
    cam.EnableWhiteBalanceAuto();
    /////////////////////////////////////////////////////////////////
    /// Базовые настройки
    /////////////////////////////////////////////////////////////////
    cam.SetExposureTime(controlData.exposure);
    cam.SetGain(controlData.gain);
    if (model_id == 23) {
        cam.SetDownsampling(XI_DWN_4x4);
        cam.SetXIAPIParamInt(XI_PRM_LIMIT_BANDWIDTH, 150);
        //cam.SetFrameRate(FPS);
    } else {
        cam.SetDownsampling(XI_DWN_2x2);
        cam.SetXIAPIParamInt(XI_PRM_LIMIT_BANDWIDTH, 1000);
    }
    /////////////////////////////////////////////////////////////////
    /// Компрессия методами OpenCV
    /////////////////////////////////////////////////////////////////
    compression_type = cv::IMWRITE_JPEG_QUALITY;
    compression_value = controlData.compression;
    ChangeCompressionParams();
    cam.SetBufferPolicy(XI_BP_UNSAFE);

    return model_id;
}

void XimeaCamera::ChangeCompressionType(int in_compression_type)
{
    compression_type = in_compression_type;
    ChangeCompressionParams();
}

void XimeaCamera::ChangeCompressionParams()
{
    params.clear();
    params.push_back(compression_type);
    params.push_back(100 - controlData.compression);
}

void XimeaCamera::Start()
{
    cam.StartAcquisition();
}
void XimeaCamera::Stop()
{
   cam.StopAcquisition();
}

XI_SWITCH XimeaCamera::Status()
{
    return cam.GetAcquisitionStatus();
}

void XimeaCamera::Close()
{
    cam.Close();
}

void XimeaCamera::ChangeGain(int gain_value)
{
    controlData.gain = gain_value;
    try
    {
        cam.SetGain(gain_value);
    }
    catch (xiAPIplus_Exception& exp)
    {        printf("Error XimeaCamera::ChangeGain:\n");
                exp.PrintError();
    }
}


void XimeaCamera::SetDownsamplingPower(XI_DOWNSAMPLING_VALUE downsamling_power)
{
    try
    {
        cam.SetDownsampling(downsamling_power);
    }
    catch (xiAPIplus_Exception& exp)
    {
        printf("Error XimeaCamera::SetDownsamplingPower:\n");
                exp.PrintError();
    }
}

void XimeaCamera::ChangeCompression(int in_compression_value)
{
    controlData.compression = in_compression_value;
    ChangeCompressionParams();
}


void XimeaCamera::ChangeExposure(int exp_value)
{
    controlData.exposure = exp_value;
    try
    {

        cam.SetExposureTime(controlData.exposure);
    }
    catch (xiAPIplus_Exception& exp)
    {
        printf("Error XimeaCamera::ChangeExposure:\n");
                exp.PrintError();
    }

}

 void XimeaCamera::ChangeImageType(XI_IMG_FORMAT img_format)
 {
     try
     {
         cam.StopAcquisition();
         cam.SetImageDataFormat(img_format);
         cam.StartAcquisition();
     }
     catch (xiAPIplus_Exception& exp)
     {
         printf("Error XimeaCamera::ChangeExposure:\n");
                 exp.PrintError();
     }
 }

Mat XimeaCamera::GetMatImage()
{
    return cam.GetNextImageOcvMat();
}
QByteArray XimeaCamera::GetUcharImage()
{
    auto img_orig = cam.GetNextImageOcvMat();
    imencode(".jpg", img_orig, buf, params);
    return QByteArray(reinterpret_cast<const char*>(buf.data()), int(buf.size()));
}
