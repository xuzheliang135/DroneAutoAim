#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <serial/serial.h>
#include "armor_finder/armor_finder.h"
#include "armor_finder/constant.h"
#include "camera/camera_wrapper.h"
#include "camera/video_wrapper.h"
#include "camera/wrapper_head.h"
#include "config.h"
//#include <unistd.h>

#include <thread>
#include <ctime>

using namespace cv;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::ios;
using std::string;

void uartReceive(Serial *uart);

int enemy_color = ENEMY_COLOR;
int main() {
    int from_camera = 1;
    bool running = true;
    Serial uart(115200);
    std::thread receive(uartReceive, &uart);
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
        ArmorFinder armor_finder(enemy_color, uart);
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

char uartReadByte(Serial &uart) {
    char byte;
    if (uart.ReadData((uint8_t *) &byte, 1) == false) {
    }
    return byte;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void uartReceive(Serial *uart) {
    char buffer[100];
    int cnt = 0;
    while (true) {
        char data;
        while ((data = uartReadByte(*uart)) != '\n') {
            buffer[cnt++] = data;
            if (cnt >= 100) {
                cnt = 0;
            }
        }
        if (cnt == 1) {
            if (buffer[0] == '2') {
                enemy_color = ENEMY_BLUE;
            } else if (buffer[0] == '1') {
                enemy_color = ENEMY_RED;
            }
        }
        cnt = 0;
    }
}

#pragma clang diagnostic pop