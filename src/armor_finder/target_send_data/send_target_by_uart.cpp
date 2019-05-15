#include "armor_finder/armor_finder.h"
#include "config.h"



bool ArmorFinder::sendTargetByUart(float x, float y, float z) {
    x = atan(x / 260) * 180 / 3.1415926;
    y = atan(y / 260) * 180 / 3.1415926;
    uart_.sendTarget(x, y, z);
    return true;
}



