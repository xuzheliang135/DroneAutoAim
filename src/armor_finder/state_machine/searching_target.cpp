#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;


bool ArmorFinder::stateSearchingTarget(cv::Mat &src_left_light) {

    /************************** find light blobs **********************************************/
    imagePreprocess(src_left_light);  // bayer hacking, to split blue and red

    pipelineForFindLightBlob(src_left_light, light_blobs_left_real_);

    /*************************** match light blobs***********************************/

    matchLightBlobVector(light_blobs_left_real_, armor_boxes_left_);
#ifdef DEBUG
    showArmorBoxVector("armor boxes", src_left_light, armor_boxes_left_);
#endif
    if (!armor_boxes_left_.empty())armor_box_left_ = armor_boxes_left_[0];
    else return false;



    /********************** convert to 3d coordinate *********************************/
    armor_space_position_.x =
            (armor_box_left_.x + armor_box_left_.width / 2 - 640 / 2);
    armor_space_position_.y = -(armor_box_left_.y + armor_box_left_.height / 2 - 480 / 2);
    //following lines are useful for aiming rotating target when the FPS is low
    //it will skip some frame when target is changing rapidly
    if (targetSearchPositionStreamControlWillSkip(armor_box_left_.x, armor_box_left_.y)) {
        return false;
    }

    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}