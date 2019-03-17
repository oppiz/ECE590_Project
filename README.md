Cloned from https://github.com/klavinslab/elma info http://klavinslab.org/elma/

# ECE590_Project
## Project Goals

The goals of this project are to port elma onto the Raspberry Pi and add priority and or multithreading functionality to elma.

This will be tested by porting elma to the Raspberry Pi and using the sense hat to create a visual display that requires quick and uninterrupted scheduler.

## Resources
This project will require Elma, Raspberry Pi sense hat library, Raspberry Pi and other Linux libraries

To Install needed library RTIMULib
```
sudo apt-get update
sudo apt-get install sense-hat
```
## Milestones
Port Elma to the Raspberry Pi. (Completed, "ASAN linking error on Pi" issue submitted and resolved on elma git hub)

Order and get Raspberry sense hat working (Completed, with example code)

Write code to control sense hat (Working sense hat code Project/LEDDisplay.cc)

Write code to improve elma scheduler (Added multithreading to elma, modifies client.cc to use new multithread)
* https://github.com/oppiz/ECE590_Project/blob/master/include/process.h
* https://github.com/oppiz/ECE590_Project/blob/master/src/process.cc
* https://github.com/oppiz/ECE590_Project/blob/master/test/multithread.cc
* https://github.com/oppiz/ECE590_Project/blob/master/src/client.cc 

Get sense hat working in/with elma scheduler (Sense has working in elma with multithreading 
* https://github.com/oppiz/ECE590_Project/blob/master/examples/LEDDisplay.cc
