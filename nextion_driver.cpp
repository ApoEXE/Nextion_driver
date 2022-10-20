//
// Created by JavierPernia-MS4M on 27/09/2022.
//
#include "nextion_driver.h"

char terminator[] ={0xff,0xff,0xff} ;
struct termios saved_attributes;
void
reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}



Nextion_driver::Nextion_driver(std::string path, int baud) {
    memset(buff,0,BUF_SIZE);
    fd = open(path.c_str(), O_RDWR | O_NOCTTY);
    if(fd < 0){
        printf("Error creating File Descriptor");
        exit(0);
    }
    printf("PATH: %s BAUD: %d FD: %d \n",path.c_str(),baud,fd);

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr (fd, &saved_attributes);
    atexit (reset_input_mode);
    struct termios tty;

    bzero(&tty, sizeof(tty));
    tty.c_cflag = baudrate(baud) | CRTSCTS | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    
    /* set input mode (non-canonical, no echo,...) */
    tty.c_lflag = 0;
         
    tty.c_cc[VTIME]    = 1;   /* inter-character timer unused */
    tty.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
        
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&tty);

      
    
  

}



void Nextion_driver::write_com(std::string key, int val) {
    // write
    //int n_written = 0;
    
    std::string value = key+std::to_string(val);
    int msg_size=value.length()+3;

    char msg[msg_size];

    strcpy(msg,value.c_str());

    msg[msg_size-3]=0xff;
    msg[msg_size-2]=0xff;
    msg[msg_size-1]=0xff;

    write(fd, (char*)msg, sizeof(msg));
    //fcntl(fd, F_SETFL, FNDELAY);
    //read(fd,(char*)buff,1);
    //printf("reading= %02x \n",buff[0]);
    
 
}




speed_t Nextion_driver::baudrate(int condition) {
    speed_t ret=0;
    switch (condition) {
        case 9600:
            ret =  (speed_t) B9600;
            break;
        case 4800:
            ret =  (speed_t) B4800;
            break;
        case 38400:
            ret = (speed_t) B38400;
            break;
    }
    return ret;
}

