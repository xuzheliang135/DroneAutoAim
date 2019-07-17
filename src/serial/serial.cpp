#include <serial/serial.h>
#include <iostream>
#include <time.h>


using namespace std;

#ifdef Windows
Serial::Serial(UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents) :
        hComm(INVALID_HANDLE_VALUE),
        portNo(6),
        parity(parity),
        databits(databits),
        stopsbits(stopsbits),
        dwCommEvents(dwCommEvents) {}

Serial::~Serial() {}

void Serial::ErrorHandler() {}

bool Serial::openPort(UINT portNo) {}

void Serial::ClosePort() {}

bool Serial::InitPort(UINT portNo, UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents) {}

UINT Serial::GetBytesInCOM() const {}

bool Serial::WriteData(const unsigned char *pData, unsigned int length) {}

bool Serial::ReadData(unsigned char *buffer, unsigned int length) {}

bool Serial::sendTarget(double x, double y, double z) {}
//Serial::Serial(UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents) :
//        hComm(INVALID_HANDLE_VALUE),
//        portNo(6),
//        parity(parity),
//        databits(databits),
//        stopsbits(stopsbits),
//        dwCommEvents(dwCommEvents) {
//    LOG_DEBUG_MSG("Waiting for serial COM");
//    while (InitPort(portNo, baud, parity, databits, stopsbits, dwCommEvents) == false);
//    LOG_DEBUG_MSG("Port COM open success!");
//}
//
//Serial::~Serial() {
//    ClosePort();
//}
//
//void Serial::ErrorHandler() {
//    LOG_DEBUG_MSG("Serial COM%d offline, waiting for serial COM");
////    while (InitPort(portNo, baud, parity, databits, stopsbits, dwCommEvents) == false);
////    LOG_DEBUG_MSG("Port COM reopen success!");
//}
//
//bool Serial::openPort(UINT portNo) {
//    char szPort[50];
//    sprintf_s(szPort, "COM%d", portNo);
//
//    /** 打开指定的串口 */
//    hComm = CreateFileA(szPort,          /** 设备名,COM1,COM2等 */
//                        GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
//                        0,                            /** 共享模式,0表示不共享 */
//                        NULL,                         /** 安全性设置,一般使用NULL */
//                        OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
//                        0,
//                        0);
//
//    return hComm != INVALID_HANDLE_VALUE;
//}
//
//void Serial::ClosePort() {
//    /** 如果有串口被打开，关闭它 */
//    if (hComm != INVALID_HANDLE_VALUE) {
//        CloseHandle(hComm);
//        hComm = INVALID_HANDLE_VALUE;
//    }
//}
//
//bool Serial::InitPort(UINT portNo, UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents) {
//    /** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
//    char szDCBparam[50];
//    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
//
//    if (!openPort(portNo)) {
//        cout << "open error!" << endl;
//        return false;
//    }
//
//    BOOL bIsSuccess = TRUE;
//    COMMTIMEOUTS CommTimeouts;
//    CommTimeouts.ReadIntervalTimeout = 0;
//    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
//    CommTimeouts.ReadTotalTimeoutConstant = 0;
//    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
//    CommTimeouts.WriteTotalTimeoutConstant = 0;
//    if (bIsSuccess) {
//        bIsSuccess = SetCommTimeouts(hComm, &CommTimeouts);
//    } else {
//        cout << "SetCommTimeouts error!" << endl;
//    }
//
//    DCB dcb;
//    if (bIsSuccess) {
//        /** 获取当前串口配置参数,并且构造串口DCB参数 */
//        bIsSuccess = GetCommState(hComm, &dcb);
//        bIsSuccess = BuildCommDCB(szDCBparam, &dcb);
//        if (!bIsSuccess) {
//
//            cout << "Create dcb fail with " << GetLastError() << endl;
//        }
//        /** 开启RTS flow控制 */
//        dcb.fRtsControl = RTS_CONTROL_ENABLE;
//    }
//
//    if (bIsSuccess) {
//        /** 使用DCB参数配置串口状态 */
//        bIsSuccess = SetCommState(hComm, &dcb);
//        if (!bIsSuccess) {
//            cout << "SetCommState error!" << endl;
//        }
//    }
//
//    /**  清空串口缓冲区 */
//    PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
//
//    return bIsSuccess;
//}
//
//UINT Serial::GetBytesInCOM() const {
//    DWORD dwError = 0;  /** 错误码 */
//    COMSTAT comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
//    memset(&comstat, 0, sizeof(COMSTAT));
//
//    UINT BytesInQue = 0;
//    /** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
//    if (ClearCommError(hComm, &dwError, &comstat)) {
//        BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
//    }
//
//    return BytesInQue;
//}
//
//bool Serial::WriteData(const unsigned char *pData, unsigned int length) {
//    if (hComm == INVALID_HANDLE_VALUE) {
//        ErrorHandler();
//        return false;
//    }
//
//    /** 向缓冲区写入指定量的数据 */
//    BOOL bResult = TRUE;
//    DWORD BytesToSend = 0;
//    bResult = WriteFile(hComm, pData, length, &BytesToSend, NULL);
//    if (!bResult) {
//        DWORD dwError = GetLastError();
//        /** 清空串口缓冲区 */
//        PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);
//        ErrorHandler();
//        return false;
//    }
//    return true;
//}
//
//bool Serial::ReadData(unsigned char *buffer, unsigned int length) {
//    if (hComm == INVALID_HANDLE_VALUE) {
//        ErrorHandler();
//        return false;
//    }
//
//    /** 从缓冲区读取length字节的数据 */
//    BOOL bResult = TRUE;
//    DWORD totalRead = 0, onceRead = 0;
//    while (totalRead < length) {
//        bResult = ReadFile(hComm, buffer, length - totalRead, &onceRead, NULL);
//        totalRead += onceRead;
//        if ((!bResult)) {
//            /** 获取错误码,可以根据该错误码查出错误原因 */
//            DWORD dwError = GetLastError();
//
//            /** 清空串口缓冲区 */
//            PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT);
//            ErrorHandler();
//            return false;
//        }
//    }
//    return bResult;
//}
//bool Serial::sendTarget(double x, double y, double z) {
//    static short x_tmp, y_tmp, z_tmp;
//    static time_t last_time = time(nullptr);
//    static int fps;
//    uint8_t buff[8];
//    time_t t = time(nullptr);
//    if (last_time != t) {
//        last_time = t;
//        cout << "fps:" << fps << ", (" << x << "," << y << "," << z << ")" << endl;
//        fps = 0;
//    }
//    fps += 1;
//
//    x_tmp = static_cast<short>(x * (32768 - 1) / 100);
//    y_tmp = static_cast<short>(y * (32768 - 1) / 100);
//    z_tmp = static_cast<short>(z * (32768 - 1) / 1000);
//
//    buff[0] = 's';
//    buff[1] = static_cast<char>((x_tmp >> 8) & 0xFF);
//    buff[2] = static_cast<char>((x_tmp >> 0) & 0xFF);
//    buff[3] = static_cast<char>((y_tmp >> 8) & 0xFF);
//    buff[4] = static_cast<char>((y_tmp >> 0) & 0xFF);
//    buff[5] = static_cast<char>((z_tmp >> 8) & 0xFF);
//    buff[6] = static_cast<char>((z_tmp >> 0) & 0xFF);
//    buff[7] = 'e';
//
//    return WriteData(buff, sizeof(buff));
//}

#elif defined(Linux)

#include <string.h>

using namespace std;

string get_uart_dev_name() {
    FILE *ls = popen("ls /dev/ttyUSB* --color=never", "r");
    char name[20] = {0};
    fscanf(ls, "%s", name);
    return name;
}

Serial::Serial(int nSpeed, char nEvent, int nBits, int nStop) :
        nSpeed(nSpeed), nEvent(nEvent), nBits(nBits), nStop(nStop) {
    LOG_DEBUG_MSG("Wait for serial be ready!");
    while (InitPort(nSpeed, nEvent, nBits, nStop) == false);
        LOG_DEBUG_MSG("Port set successfully!");
}

Serial::~Serial() {
    close(fd);
    fd = -1;
}

bool Serial::InitPort(int nSpeed, char nEvent, int nBits, int nStop) {
    string name = "/dev/ttyTHS2";//get_uart_dev_name();
    if (name == "") {
        return false;
    }
    if ((fd = open(name.data(), O_RDWR)) < 0) {
        return false;
    }
    if (set_opt(fd, nSpeed, nEvent, nBits, nStop) < 0) {
        return false;
    }
    return true;
}

//int GetBytesInCOM() const {
//
//}

bool Serial::WriteData(const unsigned char *pData, unsigned int length) {
    int cnt = 0, curr = 0;
    while ((curr = write(fd, pData + cnt, length - cnt)) > 0 && (cnt += curr) < length);
    if (cnt < 0) {
        LOG_DEBUG_MSG("Serial offline!");
        close(fd);
        return false;
    }
    return true;
}

bool Serial::ReadData(unsigned char *buffer, unsigned int length) {
    int cnt = 0, curr = 0;
    while ((curr = read(fd, buffer + cnt, length - cnt)) > 0 && (cnt += curr) < length);
    if (cnt < 0) {
        LOG_DEBUG_MSG("Serial offline!");
        close(fd);
        return false;
    }
    return true;
}

int Serial::set_opt(int fd, int nSpeed, char nEvent, int nBits, int nStop) {
    termios newtio{}, oldtio{};
    if (tcgetattr(fd, &oldtio) != 0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits) {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
        default:
            break;
    }

    switch (nEvent) {
        case 'O':  //奇校验
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':  //偶校验
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':  //无校验
            newtio.c_cflag &= ~PARENB;
            break;
        default:
            break;
    }

    switch (nSpeed) {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    if (nStop == 1) {
        newtio.c_cflag &= ~CSTOPB;
    } else if (nStop == 2) {
        newtio.c_cflag |= CSTOPB;
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");

    return 0;
}

bool Serial::sendTarget(double x, double y, double z) {
    static short x_tmp, y_tmp, z_tmp;
    static time_t last_time = time(nullptr);
    static int fps;
    uint8_t buff[8];
    time_t t = time(nullptr);
    if (last_time != t) {
        last_time = t;
        cout << "fps:" << fps << ", (" << x << "," << y << "," << z << ")" << endl;
        fps = 0;
    }
    fps += 1;

    x_tmp = static_cast<short>(x * (32768 - 1) / 100);
    y_tmp = static_cast<short>(y * (32768 - 1) / 100);
    z_tmp = static_cast<short>(z * (32768 - 1) / 1000);

    buff[0] = 's';
    buff[1] = static_cast<char>((x_tmp >> 8) & 0xFF);
    buff[2] = static_cast<char>((x_tmp >> 0) & 0xFF);
    buff[3] = static_cast<char>((y_tmp >> 8) & 0xFF);
    buff[4] = static_cast<char>((y_tmp >> 0) & 0xFF);
    buff[5] = static_cast<char>((z_tmp >> 8) & 0xFF);
    buff[6] = static_cast<char>((z_tmp >> 0) & 0xFF);
    buff[7] = 'e';

    return WriteData(buff, sizeof(buff));
}

#endif /* switch os */
