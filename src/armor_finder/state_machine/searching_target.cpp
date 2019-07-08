#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;

bool ArmorFinder::stateSearchingTarget(cv::Mat &src_light) {

    /************************** find light blobs **********************************************/
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