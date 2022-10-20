all: main.cpp nextion_driver.cpp
	g++ -o test -std=c++11 main.cpp nextion_driver.cpp -Wall