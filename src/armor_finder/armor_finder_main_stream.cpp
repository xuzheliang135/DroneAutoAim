
#include "armor_finder/armor_finder.h"
#include "config.h"
using namespace cv;


int ArmorFinder::run(cv::Mat &src) {
    src_raw_ = src.clone();
    if (src_raw_.type() == CV_8UC3) {
        cvtColor(src_raw_, src_raw_, CV_RGB2GRAY);
    }


    switch (cur_state_) {
        case SEARCHING_TARGET:
            if (stateSearchingTarget(src)) {
                target_found_frame_cnt++;
            } else {
                target_found_frame_cnt = 0;
            }
            if (target_found_frame_cnt >= state_machine_param_.THRESHOLD_FOUND_SEARCHING_TO_TRACKING) {
                if ((Rect2d(0, 0, 640, 480) & armor_box_).area() <
                    armor_box_.area()) { // avoid box touching edges
                    target_found_frame_cnt = 0;
                    break;
                }
                Mat roi_left = src_raw_.clone()(armor_box_);
                threshold(roi_left, roi_left, track_param_.THRESHOLD_FOR_COUNT_NON_ZERO, 255, THRESH_BINARY);
                total_contour_area_left_ = countNonZero(roi_left);

                trackInit(kcf_tracker_, src_raw_, armor_box_);
                transferState(TRACKING_TARGET);
                LOG_INFO(std::cout << "dive into tracking" << std::endl);
            }
            break;

        case TRACKING_TARGET:
            if (!stateTrackingTarget(src_raw_)) {
                LOG_INFO(std::cout << "jump out tracking" << std::endl);
                transferState(SEARCHING_TARGET);
            }
            break;
        case STAND_BY:
            transferState(SEARCHING_TARGET);
            break;
        default:
            cout << "incorrect state: " << cur_state_ << endl;
    }

    return 0;
}