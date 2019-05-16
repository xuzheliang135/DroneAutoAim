//
// Created by zhikun on 18-11-7.
// used for testing double cameras
// camera0 is left camera, camera1 is right camera.
//

#ifndef VIDEO_TEST1_CAMERA_WRAPPER_H
#define VIDEO_TEST1_CAMERA_WRAPPER_H

#include <stdio.h>
#include <iostream>
#include <thread>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include "wrapper_head.h"
#include "camera_api.h"

class CameraWrapper : public WrapperHead {
private:
    unsigned char *rgb_buffer{};
    int camera_cnts;
    int camera_status;
    tSdkCameraDevInfo camera_enum_list[2]{};
    int h_camera{};
    char camera_name[32]{};

    tSdkCameraCapbility tCapability{};
    tSdkFrameHead frame_info{};
    BYTE *pby_buffer{};
    IplImage *iplImage;
    int channel;

public:
    CameraWrapper();

    ~CameraWrapper() final;

    /**
     * @brief initialize the cameras, including connecting devices, setting handle and so on
     * @return
     */
    bool init() final;

    /**
     * @brief read image from cameras,
     * @param src0
     * @param src1
     * @return
     */
    bool read(cv::Mat &src0) final;

    /**
     * @brief read the image without process, it is a single channel, but it is a bayer matrix
     * @param src0
     * @param src1
     * @return
     */
    bool readRaw(cv::Mat &src0);

};


#endif //VIDEO_TEST1_CAMERA_WRAPPER_H
