//
// Created by JavierPernia-MS4M on 27/09/2022.
//
#include "nextion_driver.h"

#define MASK 0x0001
#define STARTUP_DONE 16777215
#define NEXTION_RDY 2298478591

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

void *read_thread(void *arg)
{
    char buff[BUF_SIZE];
    int _n = 0;
    double tmp = 0;
    uint16_t flag = 0;
    memset(buff, 0, BUF_SIZE);

    printf("starting reading thread\n");
    while (1)
    {
        _n = read(fd, (char *)buff, BUF_SIZE);
        printf("size %d reading= ", _n);
        if (_n > 3)
        {
            for (uint8_t i = 0; i < _n; i++)
            {
                printf(" %02x ", buff[i]);
                if (buff[i] == 0x00)
                {
                    tmp = ((uint32_t)buff[i] << 24) | ((uint32_t)buff[i + 1] << 16) | ((uint32_t)buff[i + 2] << 8) | ((uint32_t)buff[i + 3] & 0xff);
                    if (tmp == STARTUP_DONE)
                    {
                        //printf("STARTUP %lf\n", tmp);
                        flag |= 0x0001;
                    }
                }
                if (buff[i] == 0x88)
                {
                    tmp = ((uint32_t)buff[i] << 24) | ((uint32_t)buff[i + 1] << 16) | ((uint32_t)buff[i + 2] << 8) | ((uint32_t)buff[i + 3]);
                    if (tmp == NEXTION_RDY)
                    {
                        //printf("NEXTION READY %lf\n", tmp);
                        flag |= 0x0002;
                    }
                }
            }
            printf("\n");
            if(flag&MASK)
            printf("STARTUP \n");
            if(flag>>1&MASK)
            printf("NEXTION READY \n");
            memset(buff, 0, _n);
        }
    }

    pthread_exit(NULL);
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

    tty.c_cc[VTIME] = 1; /* inter-character timer unused */
    tty.c_cc[VMIN] = 3;  /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tty);

    pthread_t r_thread;
    int ret = pthread_create(&r_thread, NULL, &read_thread, NULL);
    if (ret != 0)
    {
        printf("Error: pthread_create() failed\n");
        exit(EXIT_FAILURE);
    }
}

void Nextion_driver::write_com(std::string key, int val)
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

    printf("wrote %s \n", msg);
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
    }
    return ret;
}
