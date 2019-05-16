#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "config.h"

#ifdef Windows

#include <Windows.h>

class Serial
{
public:
    Serial(UINT  baud = CBR_115200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
    ~Serial();

    bool InitPort(UINT  portNo = 1, UINT  baud = CBR_9600, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
    UINT GetBytesInCOM() const ;
    bool WriteData(const unsigned char* pData, unsigned int length);
    bool ReadData(unsigned char* buffer, unsigned int length);
private:
    bool openPort(UINT  portNo);
    void ClosePort();
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
    int fd;
    int nSpeed;
    char nEvent;
    int nBits;
    int nStop;
    char buff[8];
    int fps;
    time_t cur_time;

    int set_opt(int fd, int nSpeed, char nEvent, int nBits, int nStop);

    void countFPS();

public:
    Serial(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    ~Serial();

    bool InitPort(int nSpeed = 115200, char nEvent = 'N', int nBits = 8, int nStop = 1);

    bool WriteData(const unsigned char *pData, unsigned int length);

    bool ReadData(unsigned char *buffer, unsigned int length);

    void sendTarget(float x, float y, float z);
};

#endif

#endif /* _SERIAL_H_ */
