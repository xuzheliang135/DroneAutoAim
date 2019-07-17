#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;

void erodeAndDilate(cv::Mat &src_left) {
    static Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(1, 2));
    erode(src_left, src_left, kernel_erode);

    static Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(2, 4));
    dilate(src_left, src_left, kernel_dilate);
}

void drawRotatedRectangle(Mat &img, const RotatedRect &rect, const Scalar &s) {
    Point2f points[4];
    rect.points(points);
    for (int i = 0; i < 3; i++)line(img, points[i], points[i + 1], s);
    line(img, points[3], points[0], s);
}
bool ArmorFinder::stateSearchingTarget(cv::Mat &src) {

    /************************** find light blobs **********************************************/
    light_blobs.clear();
//    erodeAndDilate(src); //腐蚀，膨胀
    findLightBlob(src, light_blobs);
    Mat show;
    cvtColor(src, show, COLOR_GRAY2RGB);
    for (const auto &i:light_blobs) {
        drawRotatedRectangle(show, i.rect, Scalar(0, 0, 255));
        Point2d center = i.rect.center;
        double k = tan(getBlobAngel(i) * 3.14159 / 180);
        Point2d p1(center.x + 10, center.y + 10 * k), p2(center.x - 10, center.y - 10 * k);
        line(show, p1, p2, Scalar(0, 255, 0), 1);
    }
    LOG_DEBUG(imshow("light_blobs_angel", show));
    LOG_DEBUG(showBlobs("light_blobs", src, light_blobs));
    /*************************** match light blobs***********************************/
    matchLightBlobVector(light_blobs, armor_boxes_);

    LOG_DEBUG(showArmorBoxVector("armor boxes", src, armor_boxes_));
    if (!armor_boxes_.empty())armor_box_ = armor_boxes_[0];
    else return false;

    /********************** convert to 3d coordinate *********************************/
    armor_space_position_.x =
            (armor_box_.x + armor_box_.width / 2 - 640.0 / 2);
    armor_space_position_.y = -(armor_box_.y + armor_box_.height / 2 - 480.0 / 2);
    armor_space_position_.z = 0.0;
    return sendTargetByUart(
            static_cast<float>(armor_space_position_.x),
            static_cast<float>(armor_space_position_.y),
            static_cast<float>(armor_space_position_.z));
}
