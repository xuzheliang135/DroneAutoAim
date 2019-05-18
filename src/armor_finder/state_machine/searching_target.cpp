#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;

void preprocessColor_tmp(cv::Mat &src_left) {
    static Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(1, 4));
    erode(src_left, src_left, kernel_erode);

    static Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(2, 4));
    dilate(src_left, src_left, kernel_dilate);

    static Mat kernel_erode2 = getStructuringElement(MORPH_RECT, Size(2, 4));
    erode(src_left, src_left, kernel_erode2);

    static Mat kernel_dilate2 = getStructuringElement(MORPH_RECT, Size(3, 6));
    dilate(src_left, src_left, kernel_dilate2);

    float alpha = 1.5;
    int beta = 0;
    src_left.convertTo(src_left, -1, alpha, beta);
}//todo:整合两处
bool ArmorFinder::stateQuickSearchingTarget(cv::Mat &src_color) {
    preprocessColor_tmp(src_); //腐蚀，膨胀
    resize(src_, src_, Size(640, 480));
    threshold(src_, src_, light_blob_param_.GRAY_THRESH, 255, THRESH_BINARY);
    std::vector<vector<Point>> light_contours;
    std::vector<vector<Point>> res_contours;
    findContours(src_, light_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    for (auto &contour:light_contours) {
        if (contourArea(contour) > light_blob_param_.CONTOUR_AREA_MIN)
            res_contours.emplace_back(contour);
    }
    if (res_contours.size() <= 1)
        return false;
    std::vector<Point> light_points;
    for (auto &points:res_contours) {
        light_points.insert(light_points.end(), points.begin(), points.end());
    }
    RotatedRect rect = minAreaRect(light_points);

    armor_space_position_.x =
            (rect.center.x - 640.0 / 2);
    armor_space_position_.y = -(rect.center.y - 480.0 / 2);
    armor_space_position_.z = 10;
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}
bool ArmorFinder::stateSearchingTarget(cv::Mat &src_light) {

    /************************** find light blobs **********************************************/
    imagePreprocess(src_light);  // bayer hacking, to split blue and red
    pipelineForFindLightBlob(src_light, light_blobs_real_);

    /*************************** match light blobs***********************************/
    matchLightBlobVector(light_blobs_real_, armor_boxes_);
    LOG_DEBUG(showArmorBoxVector("armor boxes", src_light, armor_boxes_));
    if (!armor_boxes_.empty())armor_box_ = armor_boxes_[0];
    else return false;

    /********************** convert to 3d coordinate *********************************/
    armor_space_position_.x =
            (armor_box_.x + armor_box_.width / 2 - 640.0 / 2);
    armor_space_position_.y = -(armor_box_.y + armor_box_.height / 2 - 480.0 / 2);
    //following lines are useful for aiming rotating target when the FPS is low
    //it will skip some frame when target is changing rapidly
    if (targetSearchPositionStreamControlWillSkip(armor_box_.x, armor_box_.y)) {
        return false;
    }
    armor_space_position_.z = 0.0;
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}