
#include "armor_finder/armor_finder.h"

using namespace cv;
using std::cout;
using std::endl;

void ArmorFinder::initLightParam() {
    light_blob_param_.PREPROCESS_SUBSTRACT_FACTOR = 150;
    light_blob_param_.PREPROCESS_MULTIPLY_FACTOR = 3.5;
    light_blob_param_.GRAY_THRESH = 80;
    light_blob_param_.CONTOUR_AREA_MIN = 1;
    light_blob_param_.CONTOUR_AREA_MAX = 3000;
    light_blob_param_.CONTOUR_LENGTH_MIN = 3;
    light_blob_param_.CONTOUR_HW_RATIO_MIN = 2.5;       // 2.5
    light_blob_param_.CONTOUR_HW_RATIO_MAX = 15;
    light_blob_param_.CONTOUR_ANGLE_MAX = 20.0;
    light_blob_param_.Y_POSITION_MIN = 50;
}


void drawRotatedRectangle(Mat &img, const RotatedRect &rect, const Scalar &s) {
    Point2f points[4];
    rect.points(points);
    for (int i = 0; i < 3; i++)line(img, points[i], points[i + 1], s);
    line(img, points[3], points[0], s);
}

bool ArmorFinder::findLightBlob(const cv::Mat &src, vector <LightBlob> &light_blobs) {
    static Mat src_bin;

    threshold(src, src_bin, light_blob_param_.GRAY_THRESH, 255, THRESH_BINARY);

    std::vector <vector<Point>> light_contours;
    LOG_DEBUG(imshow("blobs bin", src_bin));
    findContours(src_bin, light_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for (auto &light_contour : light_contours) {
        if (!isValidLightContour(light_contour)) {
            continue;
        }
        light_blobs.emplace_back(light_contour);
    }
    return light_blobs.size() >= 2;
}


bool ArmorFinder::isValidLightContour(const vector <Point> &light_contour) {
    double cur_contour_area = contourArea(light_contour);
    return !(cur_contour_area > light_blob_param_.CONTOUR_AREA_MAX ||
             cur_contour_area < light_blob_param_.CONTOUR_AREA_MIN);
}
