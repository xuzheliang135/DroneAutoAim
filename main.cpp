#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <serial/serial.h>
#include "armor_finder/armor_finder.h"
#include "armor_finder/constant.h"
#include "config.h"

#include<sys/time.h>

#define GET_TIME(before, after) ((after.tv_sec-before.tv_sec)*1000000+(after.tv_usec-before.tv_usec))
#define TIME_IT(x, name) x;
//#define TIME_IT(x, name){\
//struct timeval before{},after{};\
//gettimeofday(&before, nullptr);\
//x;\
//gettimeofday(&after, nullptr);\
//cout<<name<<": "<<GET_TIME(before,after)<<endl;\
//}

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

        Mat src;
        int assemblePoint = 0;
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
            imshow("src", src);
            armor_finder.run(src);
            waitKey(1);
        }
        video.release();
        uart.~Serial();
        cout << "Program fails. Restarting" << endl;
    }
    return 0;
}
