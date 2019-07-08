#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <serial/serial.h>
#include "armor_finder/armor_finder.h"
#include "armor_finder/constant.h"
#include "config.h"

#include <ctime>

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;


int main() {
    bool running = true;
    Serial uart(115200);
    while (running) {
        VideoCapture video(0);
        Mat src, src_parallel;
        ArmorFinder armor_finder(uart);
        for (int i = 0; i < 5; i++) {
            video.read(src); // to eliminate the initial noise images
            video.read(src_parallel);
        }
        cout << "start working" << endl;
        bool ok = true;
        while (ok) {

#pragma omp parallel sections
            {
#pragma omp section
                {
                    ok = video.read(src);
                    cvtColor(src, src, COLOR_RGB2GRAY);
                }
#pragma omp section
                { armor_finder.run(src_parallel); }
            }
#pragma omp barrier


#pragma omp parallel sections
            {
#pragma omp section
                {
                    ok = video.read(src_parallel);
                    cvtColor(src_parallel, src_parallel, COLOR_RGB2GRAY);
                }
#pragma omp section
                { armor_finder.run(src); }
            }
#pragma omp barrier
            LOG_DEBUG(if (waitKey(1) == 'q')running = false);
        }
        video.release();
        cout << "Program fails. Restarting" << endl;
    }
    return 0;
}