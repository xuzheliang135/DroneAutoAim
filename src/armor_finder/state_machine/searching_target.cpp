#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;

void preprocessColor(cv::Mat &src_left) {
    static Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(1, 2));
    erode(src_left, src_left, kernel_erode);

    static Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(2, 4));
    dilate(src_left, src_left, kernel_dilate);
}

void ArmorFinder::pipelineLightBlobPreprocess(Mat &src) {
    src -= light_blob_param_.PREPROCESS_SUBSTRACT_FACTOR;
    src *= light_blob_param_.PREPROCESS_MULTIPLY_FACTOR;
    src -= light_blob_param_.PREPROCESS_SUBSTRACT_FACTOR;
    src *= light_blob_param_.PREPROCESS_MULTIPLY_FACTOR;
}

bool ArmorFinder::stateSearchingTarget(cv::Mat &src) {

    /************************** find light blobs **********************************************/
    light_blobs.clear();
//    pipelineLightBlobPreprocess(src);
//    preprocessColor(src); //腐蚀，膨胀
    findLightBlob(src, light_blobs);

    /*************************** match light blobs***********************************/
    matchLightBlobVector(light_blobs, armor_boxes_);
    showArmorBoxVector("armor boxes", src, armor_boxes_);
    if (!armor_boxes_.empty())armor_box_ = armor_boxes_[0];
    else return false;

    /********************** convert to 3d coordinate *********************************/
    armor_space_position_.x =
            (armor_box_.x + armor_box_.width / 2 - 640.0 / 2);
    armor_space_position_.y = -(armor_box_.y + armor_box_.height / 2 - 480.0 / 2);
    //following lines are useful for aiming rotating target when the FPS is low
    //it will skip some frame when target is changing rapidly
    //if (targetSearchPositionStreamControlWillSkip(armor_box_.x, armor_box_.y)) {
    //    return false;
    // }
    armor_space_position_.z = 0.0;
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}
