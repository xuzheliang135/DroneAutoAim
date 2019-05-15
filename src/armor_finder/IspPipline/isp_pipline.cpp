#include "armor_finder/armor_finder.h"

using namespace cv;


void ArmorFinder::imagePreprocess(cv::Mat &src_input) {
    if (src_input.type() == CV_8UC1) {
        splitBayerBG(src_input, src_blue0, src_red0);
        if (enemy_color_ == ENEMY_RED) {
            src_ = src_red0 - src_blue0;
        } else if (enemy_color_ == ENEMY_BLUE) {
            src_ = src_blue0 - src_red0;
        }

    } else if (src_input.type() == CV_8UC3) {
        std::vector <Mat> channels_left;
        split(src_input, channels_left);
        resize(channels_left.at(0), src_blue0, Size(SRC_WIDTH, SRC_HEIGHT));
        resize(channels_left.at(2), src_red0, Size(SRC_WIDTH, SRC_HEIGHT));
        if (enemy_color_ == ENEMY_RED) {
            src_ = src_red0;
        } else if (enemy_color_ == ENEMY_BLUE) {
            src_ = src_blue0;
        }
    }

}