#include "armor_finder/armor_finder.h"

using namespace cv;


void ArmorFinder::trackInit(KCFTracker &kcf_tracker, cv::Mat &src, cv::Rect2d &armor_box) {
    kcf_tracker.init(armor_box, src);
}