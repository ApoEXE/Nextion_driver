//
// Created by JavierPernia-MS4M on 27/09/2022.
//

#ifndef NEXTION_DRIVER_H
#define NEXTION_DRIVER_H

#include <string>
#include <unistd.h>
#include <termios.h> // POSIX terminal control definitions
#include <fcntl.h> // File control definitions
#include <stdlib.h>
#include <cstring>
#include <sstream>

#include <pthread.h>

#include <iostream>
#include <thread>
#include <functional>

#define BUF_SIZE 20
#define OVERFLOW 0X24//4 0x24 0xFF 0xFF 0xFF
#define TOUCH_EVENT 0X65//7 0x65 0x00 0x01 0x01 0xFF 0xFF 0xFF
#define CURR_PAGE 0X66//5 0x66 0x01 0xFF 0xFF 0xFF
#define TOUCH_COOR_AWAKE 0X67//9 0x67 X:[0x00 0x7A] Y:[0x00 0x1E] EVENT:[0x01] 0xFF 0xFF 0xFF
#define TOUCH_COOR_SLEEP 0X68//9 0x68 0x00 0x7A 0x00 0x1E 0x01 0xFF 0xFF 0xFF
#define STRING_DATA_ENCLOSED 0X70//VARIED
#define NUMERIC_DATA_ENCLOSED 0X71//8 0x71 0x01 0x02 0x03 0x04 0xFF 0xFF 0xFF
#define AUTO_ENTERED_SLEEP_MODE 0X86//4 0x86 0xFF 0xFF 0xFF
#define AUTO_WAKE_FROM_SLEEP 0X87//4 0x87 0xFF 0xFF 0xFF

#define START_MICROSD_UPGRADE 0X89//4 0x89 0xFF 0xFF 0xFF
#define TRANSPARENT_DATA_FINISHED 0XFD //4 0xFD 0xFF 0xFF 0xFF
#define TRANSPARENT_DATA_READY 0XFE//4 0xFE 0xFF 0xFF 0xFF



class Nextion_driver{

public:

    Nextion_driver(std::string path, int baudrate);
    ~Nextion_driver();
    void write_text(std::string key, int val);
    void write_value(std::string key, int val);
    void write_waveform(int id, int ch, int val);
    long map(long x, long in_min, long in_max, long out_min, long out_max);
    void infiniteThread(std::function<void(char[20])> callback);

private:
    char buff[BUF_SIZE];
    speed_t baudrate(int);



};
#endif //HELLO_CMAKE_CS_VIMS_H