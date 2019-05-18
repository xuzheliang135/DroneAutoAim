//
// Created by zhikun on 18-11-11.
// this file used for testing detecting armor by light blob
// testing armor is large armor
//

#ifndef STEREOVISION_FROM_VIDEO_FILE_ARMOR_FINDER_H
#define STEREOVISION_FROM_VIDEO_FILE_ARMOR_FINDER_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <armor_finder/param_struct_define.h>
#include "armor_finder/constant.h"
#include <tracker/kcftracker.hpp>
#include "tracker/tracker.h"
#include "serial/serial.h"


using std::vector;

/**
 * @brief   a class to find armor in image. SJTU-TPP@RoboMaster2019
 */
class ArmorFinder {
public:
    explicit ArmorFinder(int &color, Serial &u);

    ~ArmorFinder() = default;

    cv::Mat src_;
    cv::Mat src_blue, src_red;
    cv::Mat src_raw_;

    /**
     * @brief the interface of armor_finder
     * @param src : input
     * @param src_right : input
     * @return : bool value: whether it success.
     */
    int run(cv::Mat &src);

private:
    /**
     * parameter structures, defined in param_struct_define.h
     */
    LightBlobParam light_blob_param_{};
    LightCoupleParam light_couple_param_{};
    StereoCameraPara stereo_camera_param_{};
    ArmorSeekingParam armor_seeking_param_{};
    StateMachineParam state_machine_param_{};
    TrackingParam track_param_{};

    /**
     * vectors to store light blobs
     */
    std::vector<LightBlob> light_blobs_light_;
    std::vector<LightBlob> light_blobs_color_;
    std::vector <LightBlob> light_blobs_real_;

    /**
     * Rects to store the found armor box position
     */
    cv::Rect2d armor_box_;

    /**
     * Rects list to store all the possible armor box position
     */
    std::vector <cv::Rect2d> armor_boxes_;

    /**
     * a counter for changing state from searching to tracking
     */
    int target_found_frame_cnt;

    /**
     * Finite state machine that switch between searching and tracking
     */
    StateMachine cur_state_;

    /**
     * Wrapped class to send message to lower computer
     */
    Serial uart_;

    /**
     * A well functioned class for tracking
     */
    KCFTracker kcf_tracker_;

    /**
     * enemy color define by two constant, defined in constant.h
     */
    int &enemy_color_;

    /**
     * variable to determine the exit condition of tracking
     */
    double total_contour_area_left_;

    /**
     * about armor space positions
     */
    cv::Point3d armor_space_position_;
    /**
     * variable to store the position difference between current and last
     */
    double position_diff = 0;


private:
    /**
     * initialize parameters, the definition should be in their related module respectively
     */
    void initLightParam();

    void initLightCoupleParam();

    void initCameraParam();

    void initArmorSeekingParam();

    void initArmorPredictParam();

    void initStateMachineParam();

    void initTrackingParam();

    /**
     * @brief transfer FSM between searching and tracking, and reset the recording variables.
     * @param state
     */
    void transferState(StateMachine state);

    /**
     * @brief searching state, it will search the entire frame to try to find a target
     * @param src_light
     * @param src_right
     * @return
     */
    bool stateSearchingTarget(cv::Mat &src_light);

    bool stateQuickSearchingTarget(cv::Mat &src_color);

    /**
     * @brief tracking state, it will tracking the given area until the condition is not met
     * @param src
     * @param src_right
     * @return
     */
    bool stateTrackingTarget(cv::Mat &src);

    /**
     * @brief split the unprocessed bayer matrix into blue and red. The blue and red are only 1/4 of the raw image
     * @param src : raw bayer matrix 640*480
     * @param blue : blue part 320*240
     * @param red : red part 320*240
     */
    void splitBayerBG(cv::Mat &src, cv::Mat &blue, cv::Mat &red);

    /**
     * @brief some preprocess of image, make image from camera and video file the same.
     * @param src_input
     * @param src_right
     */
    void imagePreprocess(cv::Mat &src_input);

    /**
     * @brief find light blobs from the images.
     * @param src_light :image
     * @param src_right :image
     * @param light_blobs_real
     * @param light_blobs_real_right
     * @return
     */
    bool pipelineForFindLightBlob(cv::Mat &src_light, std::vector<LightBlob> &light_blobs_real);

    /**
     * @brief process for find light blobs, enlarge the difference between the dark and light, to highlight the light blob
     * @param InOutput
     */
    void pipelineLightBlobPreprocess(cv::Mat &InOutput);

    /**
     * @brief control the message stream to lower computer in searching state, some large jump will be skipped
     * @param x
     * @param y
     * @return
     */
    bool targetSearchPositionStreamControlWillSkip(double x, double y);

    /**
     * @brief control the message stream to lower computer in tracking state, we can do some prediction in tracking,
     *          because in tracking state the target almost moves smoothly
     * @param armor_position
     * @return
     */
    bool targetTrackPositionStreamControl(cv::Point3d &armor_position);

public:
    /**
     * @brief When there are many possible armors, it matches the most possible pair
     * @param armor_box_list_left
     * @param armor_box_list_right
     * @param armor_box_left
     * @param armor_box_right
     * @return
     */

public:

    /**
     * a function to clear all vector for searching
     */
    void clear_light_blobs_vector();


public:
    /**
     * @name    findLightBlob()
     * @brief   Find out all the light blobs in given image
     * @param   src: input image
     * @param   light_blobs: output vector of light blobs
     * @return  bool value: whether it finds more than 2 valid light blobs
     */
    bool findLightBlob(const cv::Mat &src, std::vector <LightBlob> &light_blobs);

public:

    /**
     * @brief match light blobs to find a possible armor
     * @param light_blobs
     * @param armor_box
     * @return
     */
    bool matchLightBlobVector(std::vector <LightBlob> &light_blobs, vector <cv::Rect2d> &armor_box);

public:
    /**
     * @brief send target position to lower computer by uart
     * @param x
     * @param y
     * @param z
     * @return
     */
    bool sendTargetByUart(float x, float y, float z);

private:
    /**
     * @name    isValidLightContour()
     * @brief   judge a contour whether valid or not
     * @param   light_contour: input light contour
     * @return  bool value: whether the light contour is valid
     */
    bool isValidLightContour(const vector <cv::Point> &light_contour);


    /**
     * @name    isCoupleLight()
     * @brief   judge two light blobs are a couple or not
     * @param   light_blob_x: input light blob x
     * @param   light_blob_y: input light blob y
     * @return  bool value: whether the two light blob is a couple
     */
    bool isCoupleLight(const LightBlob &light_blob_x, const LightBlob &light_blob_y);


public:
    /**
     * @brief all those show*** functions are used to display the images with found light blobs or armor rect.
     * @param windows_name
     * @param src
     * @param src1
     */
    void showImage(std::string windows_name, const cv::Mat &src);

    void showContours(std::string windows_name,
                      const cv::Mat &src, const std::vector<LightBlob> &light_blobs_left);


    void showArmorBox(std::string windows_name, const cv::Mat &src_left, const cv::Rect2d &armor_box_left);

    void
    showArmorBoxVector(std::string windows_name, const cv::Mat &src_left, const vector <cv::Rect2d> &armor_box_left);

    /**
     * @brief give the tracker an area to let it track
     * @param kcf_tracker
     * @param src
     * @param armor_box
     */
    void trackInit(KCFTracker &kcf_tracker, cv::Mat &src, cv::Rect2d &armor_box);

    /**
     * @brief the tracker will give where the area is in given image
     * @param kcf_tracker
     * @param src
     * @param armor_box
     * @return
     */
    bool track(KCFTracker &kcf_tracker, cv::Mat &src, cv::Rect2d &armor_box);

};


#endif //STEREOVISION_FROM_VIDEO_FILE_ARMOR_FINDER_H
