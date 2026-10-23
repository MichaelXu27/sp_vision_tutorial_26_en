#include "camera.hpp"

namespace io
{
    Camera::Camera(int exposure)
    {
        exposure_ms_ = exposure;
        CameraSdkInit(1);
        iStatus = CameraEnumerateDevice(&tCameraEnumList, &iCameraCounts);

        // no camera is plugged in
        if (iCameraCounts == 0)
            throw std::runtime_error("Not found camera!");

        // Start up the camera. You must do this before using any other camera function
        iStatus = CameraInit(&tCameraEnumList, -1, -1, &hCamera);
        CameraGetCapability(hCamera, &tCapability);
        g_pRgbBuffer = (unsigned char *)malloc(
            tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);
        CameraSetAeState(hCamera, FALSE); // turn off auto exposure (auto brightness)
        CameraSetExposureTime(hCamera, exposure_ms_ * 1e3);
        CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_BGR8);
        CameraPlay(hCamera);
    };

    void Camera::read(cv::Mat &img)
    {
        if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
        {
            CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer, &sFrameInfo);
            cv::Mat raw_img(cv::Size(sFrameInfo.iWidth, sFrameInfo.iHeight), CV_8UC3, g_pRgbBuffer);
            img = raw_img;
            CameraReleaseImageBuffer(hCamera, pbyBuffer);
        }
        else
            return;
    };

    Camera::~Camera()
    {
        CameraUnInit(hCamera);
        // Note: shut the camera down first, then free the memory
        free(g_pRgbBuffer);
    }

} // namespace io