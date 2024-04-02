//
// Created by JavierPernia-MS4M on 27/09/2022.
//
#include "nextion_driver.h"

char STARTUP[] = {0x00, 0x00, 0x00, 0xff, 0xff, 0xff};
char NEXTION_READY[] = {0x88, 0xff, 0xff, 0xff};

struct termios saved_attributes;
static int fd;

void reset_input_mode(void)
{
    tcsetattr(fd, TCSANOW, &saved_attributes);
    printf("RESET MODE RESTORING");
}

Nextion_driver::~Nextion_driver()
{
    close(fd);
}


Nextion_driver::Nextion_driver(std::string path, int baud)
{
    memset(buff, 0, BUF_SIZE);

    fd = open(path.c_str(), O_RDWR | O_NOCTTY);

    if (fd < 0)
    {
        printf("Error creating File Descriptor");
        exit(0);
    }

    printf("PATH: %s BAUD: %d FD: %d \n", path.c_str(), baud, fd);

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr(fd, &saved_attributes);
    atexit(reset_input_mode);
    struct termios tty;

    bzero(&tty, sizeof(tty));
    tty.c_cflag = baudrate(baud) | CRTSCTS | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    tty.c_lflag = 0;

    tty.c_cc[VTIME] = 10; /* inter-character timer unused */
    tty.c_cc[VMIN] = 1;   /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tty);
}

// Function to be called from the infinite thread
void Nextion_driver::infiniteThread(std::function<void(char[20])> callback)
{
    // Do some work in the thread
    //printf("starting reading thread\n");
    char buff[BUF_SIZE];
    memset(buff, 0, BUF_SIZE);
    while (true)
    {
        read(fd, (char *)buff, BUF_SIZE);
        //printf("size %d reading= %s ", _n,buff);
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        // Call the callback function
        callback(buff);
        memset(buff, 0, BUF_SIZE);
    }
}

void Nextion_driver::write_text(std::string key, int val)
{
    // write
    std::string value = key + "\"" + std::to_string(val) + "\"";
    int msg_size = value.length() + 3;
    char msg[msg_size];

    strcpy(msg, value.c_str());

    msg[msg_size - 3] = 0xff;
    msg[msg_size - 2] = 0xff;
    msg[msg_size - 1] = 0xff;

    write(fd, (char *)msg, sizeof(msg));

    //printf("wrote %s \n", msg);
}
void Nextion_driver::write_value(std::string key, int val)
{
    // write
    std::string value = key + std::to_string(val);
    int msg_size = value.length() + 3;
    char msg[msg_size];

    strcpy(msg, value.c_str());

    msg[msg_size - 3] = 0xff;
    msg[msg_size - 2] = 0xff;
    msg[msg_size - 1] = 0xff;

    write(fd, (char *)msg, sizeof(msg));

    //printf("wrote %s \n", msg);
}

void Nextion_driver::write_waveform(int id, int ch, int val)
{
    // write
    std::string value = "add " + std::to_string(id) + "," + std::to_string(ch) + "," + std::to_string(val);
    int msg_size = value.length() + 3;
    char msg[msg_size];

    strcpy(msg, value.c_str());

    msg[msg_size - 3] = 0xff;
    msg[msg_size - 2] = 0xff;
    msg[msg_size - 1] = 0xff;

    write(fd, (char *)msg, sizeof(msg));

    //printf("wrote %s \n", msg);
}

long Nextion_driver::map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

speed_t Nextion_driver::baudrate(int condition)
{
    speed_t ret = 0;
    switch (condition)
    {
    case 9600:
        ret = (speed_t)B9600;
        break;
    case 4800:
        ret = (speed_t)B4800;
        break;
    case 38400:
        ret = (speed_t)B38400;
        break;
    case 115200:
        ret = (speed_t)B115200;
        break;
    }
    return ret;
}
