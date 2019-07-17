#include "armor_finder/armor_finder.h"
#include "config.h"

using namespace cv;
using namespace std;


void ArmorFinder::initLightCoupleParam() {
    light_couple_param_.TWIN_ANGEL_MAX = 10;
    light_couple_param_.TWIN_LENGTH_RATIO_MAX = 4.0;
    light_couple_param_.TWIN_DISTANCE_N_MIN = 1.3;       // 1.7
    light_couple_param_.TWIN_DISTANCE_N_MAX = 3.8;       // 3.8
    light_couple_param_.TWIN_DISTANCE_T_MAX = 1.4;
    light_couple_param_.TWIN_AREA_RATIO_MAX = 3;
    light_couple_param_.TWIN_CENTER_POSITION_DIFF = 100;
}

void ArmorFinder::initArmorSeekingParam() {
    armor_seeking_param_.BORDER_IGNORE = 10;
    armor_seeking_param_.BOX_EXTRA = 10;
}

double ArmorFinder::getBlobAngel(const LightBlob &blob) {
    return blob.rect.size.width > blob.rect.size.height ? blob.rect.angle :
           blob.rect.angle - 90;
}

bool angelJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    float angle_i = ArmorFinder::getBlobAngel(light_blob_i);
    float angle_j = ArmorFinder::getBlobAngel(light_blob_j);
    return abs(angle_i - angle_j) < 10;
}
//bool angelJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
//    double angle_i = ArmorFinder::getBlobAngel(light_blob_i);
//    double angle_j = ArmorFinder::getBlobAngel(light_blob_j);
//    double res=abs(tan(angle_i+3.1415926/180)*tan(angle_j+3.1415926/180)+1);
//    return 0.18<res&&res<0.4;
//}

bool heightJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    Point2f centers = light_blob_i.rect.center - light_blob_j.rect.center;
    return abs(centers.y) < 30;
}

bool lengthJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    double side_length;
    Point2f centers = light_blob_i.rect.center - light_blob_j.rect.center;
    side_length = sqrt(centers.ddot(centers));

    return (side_length / light_blob_i.length < 6 && side_length / light_blob_i.length > 1);
}

bool lengthRatioJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    return (light_blob_i.length / light_blob_j.length < 2
            && light_blob_i.length / light_blob_j.length > 0.5);
}

bool isInVision(Rect rect) {
    Rect vision(0, 0, SRC_WIDTH, SRC_HEIGHT);
    return vision.contains(rect.br()) && vision.contains(rect.tl());
}

bool ArmorFinder::matchLightBlobVector(std::vector <LightBlob> &light_blobs, vector <cv::Rect2d> &armor_boxes) {
    armor_boxes.clear();
    if (light_blobs.size() < 2)
        return false;
    long light_index_left = -1;
    long light_index_right = -1;

    sort(light_blobs.begin(), light_blobs.end(),
         [](LightBlob a, LightBlob b) -> bool { return a.rect.center.y > b.rect.center.y; });

    for (long i = 0; i < light_blobs.size() - 1; ++i) {
        for (long j = i + 1; j < light_blobs.size(); ++j) {
            if (!isCoupleLight(light_blobs.at(i), light_blobs.at(j))) {
                continue;
            }
            light_index_left = i;
            light_index_right = j;
            Rect2d rect_left = light_blobs.at(static_cast<unsigned long>(light_index_left)).rect.boundingRect();
            Rect2d rect_right = light_blobs.at(static_cast<unsigned long>(light_index_right)).rect.boundingRect();
            if (!isInVision(rect_left) || !isInVision(rect_right))continue;

            Point2d tl(min(rect_left.x, rect_right.x), min(rect_left.y, rect_right.y));
            Point2d br(max(rect_left.br().x, rect_right.br().x), max(rect_left.br().y, rect_right.br().y));
            tl.x -= armor_seeking_param_.BOX_EXTRA;
            tl.y -= armor_seeking_param_.BOX_EXTRA;
            br.x += armor_seeking_param_.BOX_EXTRA;
            br.y += armor_seeking_param_.BOX_EXTRA;
            armor_boxes.emplace_back(Rect2d(tl, br));
        }

    }
    return light_index_left + light_index_right != -2;


}

//bool ArmorFinder::isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
//    return lengthRatioJudge(light_blob_i, light_blob_j) &&
//           lengthJudge(light_blob_i, light_blob_j) &&
//           heightJudge(light_blob_i, light_blob_j) &&
//           angelJudge(light_blob_i, light_blob_j);
//}
bool ArmorFinder::isCoupleLight(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    return lengthRatioJudge(light_blob_i, light_blob_j) &&
           lengthJudge(light_blob_i, light_blob_j) &&
           heightJudge(light_blob_i, light_blob_j) &&
           angelJudge(light_blob_i, light_blob_j);
}