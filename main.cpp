#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <serial/serial.h>
#include "armor_finder/armor_finder.h"
#include "armor_finder/constant.h"
#include "config.h"

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;

int main() {

    bool running = true;

    while (running) {
        Serial uart(115200);

        VideoCapture video(0);
//        video.set(6, CV_FOURCC('M','J','P','G'));

        Mat src;
        ArmorFinder armor_finder(uart);
        for (int i = 0; i < 5; ++i) {
            video.read(src);
        }
        cout << "start working" << endl;
        bool ok = true;
        while (ok) {
            ok = video.read(src);
            if (!ok)continue;
            cvtColor(src, src, COLOR_RGB2GRAY);
            LOG_DEBUG(imshow("src", src));
            armor_finder.run(src);
            waitKey(1);
        }
        video.release();
        uart.~Serial();
        cout << "Program fails. Restarting" << endl;
    }
    return 0;
}
