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
#define BUF_SIZE 20


class Nextion_driver{
private:
    int fd;
public:
    Nextion_driver(std::string path, int baudrate);
    virtual ~Nextion_driver(){close(fd);};
    void write_com(std::string key, int val);

private:
    char buff[BUF_SIZE];
    speed_t baudrate(int);


};
#endif //HELLO_CMAKE_CS_VIMS_H