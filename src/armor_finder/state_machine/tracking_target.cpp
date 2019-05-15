#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;

#include <math.h>

void ArmorFinder::initTrackingParam() {
    track_param_.THRESHOLD_FOR_COUNT_NON_ZERO = 200;
    track_param_.TRANSFER_RATIO_OF_TRACKING_AREA_NONZERO = 0.5;

}


bool ArmorFinder::stateTrackingTarget(cv::Mat &src_left) {
    static int pictureCount = 0;
    if (pictureCount++ > 20) {
        cv::Mat roi = src_left(armor_box_left_).clone();
        cv::resize(roi, roi, cv::Size(48, 36));
        cv::cvtColor(roi, roi, CV_BayerGR2BGR);
        if (classifier(roi) == 0) {
            pictureCount = 0;
            return false;
        }
        pictureCount = 0;
    }
    /********************** tracking ***********************************************/
    track(kcf_tracker_left_, src_left, armor_box_left_);
    if ((Rect2d(0, 0, 640, 480) & armor_box_left_).area() < armor_box_left_.area()) // avoid box touching edges
    {
        pictureCount = 0;
        return false;
    }

    Mat roi_left = src_left.clone()(armor_box_left_);
    threshold(roi_left, roi_left, track_param_.THRESHOLD_FOR_COUNT_NON_ZERO, 255, THRESH_BINARY);

    if (abs(countNonZero(roi_left) - total_contour_area_left_) >=
        track_param_.TRANSFER_RATIO_OF_TRACKING_AREA_NONZERO * total_contour_area_left_) {
        pictureCount = 0;
        return false;
    }
#ifdef DEBUG
    showArmorBox("tracking boxes", src_left, armor_box_left_);
#endif
    /********************** convert to angel *********************************/
    armor_space_position_.x =
            (armor_box_left_.x + armor_box_left_.width / 2 - 640 / 2);
    armor_space_position_.y = -(armor_box_left_.y + armor_box_left_.height / 2 - 480 / 2);
    /*************** a predict function for moving target with only constant speed *******************/
    targetTrackPositionStreamControl(armor_space_position_);

    /********************** send it by uart and adjust the original point to the center *************/
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}