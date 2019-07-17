#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;

#include <math.h>

void ArmorFinder::initTrackingParam() {
    track_param_.THRESHOLD_FOR_COUNT_NON_ZERO = 200;
    track_param_.TRANSFER_RATIO_OF_TRACKING_AREA_NONZERO = 0.4;

}

bool ArmorFinder::stateTrackingTarget(cv::Mat &src) {
    cv::Mat roi = src(armor_box_).clone();
    /********************** tracking ***********************************************/
    track(kcf_tracker_, src, armor_box_);
    if ((Rect2d(0, 0, 640, 480) & armor_box_).area() < armor_box_.area())
        return false;// avoid box touching edges

    threshold(roi, roi, track_param_.THRESHOLD_FOR_COUNT_NON_ZERO, 255, THRESH_BINARY);

    if (abs(countNonZero(roi) - total_contour_area_left_) >=
        track_param_.TRANSFER_RATIO_OF_TRACKING_AREA_NONZERO * total_contour_area_left_) {
        return false;
    }
    LOG_DEBUG(showArmorBox("tracking boxes", src, armor_box_));
    /********************** convert to angel *********************************/
    armor_space_position_.x =
            (armor_box_.x + armor_box_.width / 2 - 640.0 / 2);
    armor_space_position_.y = -(armor_box_.y + armor_box_.height / 2 - 480.0 / 2);

    /********************** send it by uart and adjust the original point to the center *************/
    armor_space_position_.z = 0.0;
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}