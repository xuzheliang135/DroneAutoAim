//
// Created by zhikun on 18-12-2.
//

#ifndef STEREOVISION_UART_H
#define STEREOVISION_UART_H

#include <stdio.h>
#include <time.h>
#include <iostream>

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

//#include <opencv2/core/core.hpp>


/**
 * The uart is used to send messages to lower computer
 */
class Uart {
private:
    int fd;
    char buff[8];
    int fps;
    time_t cur_time;

    int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);

public:
    Uart();

    void sendTarget(float x, float y, float z);

    void countFPS();

};


#endif //STEREOVISION_UART_H
