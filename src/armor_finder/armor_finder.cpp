
#include <armor_finder/armor_finder.h>
#include <serial/serial.h>

using namespace cv;
using std::cout;
using std::endl;
using std::vector;

ArmorFinder::ArmorFinder(Serial &u, int &target, int &enemyColor) :
        uart_(u),
        target(target),
        enemyColor(enemyColor),
        kcf_tracker_(false, true, false, false) {
    initLightParam();
    initLightCoupleParam();
    initStateMachineParam();
    initTrackingParam();

    cur_state_ = SEARCHING_TARGET;
    target_found_frame_cnt = 0;
    total_contour_area_left_ = 0;
    position_diff = 0;
}

