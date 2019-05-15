#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "armor_finder/armor_finder.h"
#include "armor_finder/constant.h"
#include "camera/camera_wrapper.h"
#include "camera/video_wrapper.h"
#include "camera/wrapper_head.h"
#include "config.h"
#include <unistd.h>

#include <ctime>

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;


int main() {
    int from_camera = 0;
    bool running = true;
    while (running) {

        WrapperHead *video;

        if (from_camera)
            video = new CameraWrapper;
        else
            video = new VideoWrapper(
                    "/home/xuzheliang135/video.avi");

        if (video->init())
            cout << "Video source initialization successfully." << endl;
        else continue;
        Mat src, src_parallel;
        ArmorFinder armor_finder("/mnt/e/C/RoboMaster/sjtu_rm2019_winter_version-master/tools/para/");
        armor_finder.setEnemyColor(ENEMY_COLOR);
        for (int i = 0; i < 5; i++) {
            video->read(src); // to eliminate the initial noise images
            video->read(src_parallel);
        }
        cout << "start working" << endl;
        bool ok = true;
        while (ok) {

#pragma omp parallel sections
            {
#pragma omp section
                { ok = video->read(src); }
#pragma omp section
                { armor_finder.run(src_parallel); }
            }
#pragma omp barrier


#pragma omp parallel sections
            {
#pragma omp section
                { ok = video->read(src_parallel); }
#pragma omp section
                { armor_finder.run(src); }
            }
#pragma omp barrier
            LOG_DEBUG(
            if (waitKey(1) == 'q')running = false);
        }
        delete video;
        cout << "Program fails. Restarting" << endl;
    }
    return 0;
}