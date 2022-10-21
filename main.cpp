// g++ -o test -std=c++11 main.cpp nextion_driver.cpp -Wall

#include "nextion_driver.h"



int main(){

    Nextion_driver display = Nextion_driver("/dev/ttyTHS1",9600);
    int inc = 0;
    while (inc <= 100)
    {
        display.write_com("n0.val=",inc);
        inc++;
        sleep(1);
    }
    
    
    return 0;
}