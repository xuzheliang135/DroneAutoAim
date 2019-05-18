#ifndef _SERIAL_H_
#define _SERIAL_H_


#include "config.h"

#ifdef Windows

//#include <Windows.h>
#include <camera/camera_define.h>
//#include <winbase.h>
class Serial
{
public:
    Serial(UINT  baud = 115200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = 0x1);
    ~Serial();
    void sendTarget(float x, float y, float z);
    bool ReadData(unsigned char *buffer, unsigned int length);
private:
    HANDLE hComm;
    UINT portNo;
    UINT baud;
    char parity;
    UINT databits;
    UINT stopsbits;
    DWORD dwCommEvents;
};

#elif defined(Linux)

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <ctime>

class Serial {
private:
    int fd{};
    int nSpeed;
    char nEvent;
    int nBits;
    int nStop;
    char buff[8]{};
    int fps{};
    time_t cur_time{};

    int set_opt(int fd, int nSpeed, char nEvent, int nBits, int nStop);

    void countFPS();

public:
    explicit Serial(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    ~Serial();

    bool InitPort(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    bool WriteData(const unsigned char *pData, unsigned int length);

    bool ReadData(unsigned char *buffer, unsigned int length);

    void sendTarget(float x, float y, float z);
};

#endif

#endif /* _SERIAL_H_ */
