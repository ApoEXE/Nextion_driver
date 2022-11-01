// g++ -o test -std=c++11 main.cpp nextion_driver.cpp -Wall

#include "nextion_driver.h"
#include <time.h> // So we can use time() function
#include <cmath>

int main(){

    Nextion_driver display = Nextion_driver("/dev/ttyTHS1",9600);
    int inc = 0;
    srand((unsigned) time(NULL)); //Randomize seed initialization
    display.write_com("cle 6,0");
    while (1)
    {
        display.write_com("voltage1.val=",inc);
        inc++;
        
        int randNum = rand() % 10; // Generate a random number between 0 and 9
        float value = sin(randNum)*100;
        display.write_com("add 6,0,",value+120);
        sleep(1);
    }
    
    
    return 0;
}