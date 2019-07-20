#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>

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

int get_dev_name() {
    FILE *ls = popen("ls /dev/video* --color=never", "r");
    int name = 0;
    fscanf(ls, "/dev/video%d", &name);
    return name;
}

int enemy_color = ENEMY_RED;
int target = INFANTRY;

void uartReceive(Serial *uart);
int main() {
    bool running = true;
    while (running) {
        Serial uart(115200);
        std::thread receive(uartReceive, &uart);

        VideoCapture video(get_dev_name());
        cout << "device number: " << get_dev_name() << endl;
//        video.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
//        video.set(CV_CAP_PROP_BRIGHTNESS,0);//亮度
//        video.set(CV_CAP_PROP_EXPOSURE,0.01);//曝光
//        video.set(CV_CAP_PROP_FRAME_WIDTH,640);
//        video.set(CV_CAP_PROP_FRAME_HEIGHT,480);
//        video.set(CV_CAP_PROP_FPS,120);

        Mat src;
        ArmorFinder armor_finder(uart, target, enemy_color);
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

char uartReadByte(Serial &uart) {
    char byte;
    if (!uart.ReadData((uint8_t *) &byte, 1)) {
        LOG_DEBUG_MSG("serial error!");
    }
    return byte;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void uartReceive(Serial *uart) {
    char buffer[50];
    int cnt = 0;
    while (true) {
        char data;
        while ((data = uartReadByte(*uart)) != '\n') {
            buffer[cnt++] = data;
            if (cnt >= 50) {
                cnt = 0;
            }
        }
        if (cnt == 1) {
            if (buffer[0] == '2') {
                enemy_color = ENEMY_BLUE;
            } else if (buffer[0] == '1') {
                enemy_color = ENEMY_RED;
            } else if (buffer[0] == '3') {
                target = INFANTRY;
            } else if (buffer[0] == '4') {
                target = BASE;
            }
        }
        cnt = 0;
    }
}

#pragma clang diagnostic pop
