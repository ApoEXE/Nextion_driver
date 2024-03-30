// g++ -o test -std=c++11 main.cpp nextion_driver.cpp -Wall

#include "nextion_driver.h"

// Example callback function
void callbackFunction(char *msg)
{
    std::cout << "MSG: " << msg << std::endl;
}

int main()
{

    Nextion_driver display = Nextion_driver("/dev/ttyTHS1", 115200);
    // Create a thread with the infiniteThread function and pass the callback function
    std::thread t(std::bind(&Nextion_driver::infiniteThread, &display, callbackFunction));
    int inc = 0;
    while (inc <= 100)
    {
        // display.write_text("t0.txt=",inc);
        display.write_value("x0.val=", (inc * 100));
        long mapped = display.map(inc, 0, 1023, 0, 255);
        display.write_waveform(10, 0, (uint8_t)mapped);
        inc++;
        sleep(1);
    }
    t.join(); // Wait for the thread to finish (which it won't, in this case)

    return 0;
}