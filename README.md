Cloned from https://github.com/klavinslab/elma info http://klavinslab.org/elma/

# ECE590_Project
## Project Goals

The goals of this project are to port elma onto the Raspberry Pi and add multithreading functionality to elma.

This will be tested by porting elma to the Raspberry Pi and using the sense hat to create a visual display that requires quick and uninterrupted scheduler.

![alt text](https://github.com/oppiz/ECE590_Project/blob/master/PiSensehat%20Example.jpg?raw=true)

## Resources
This project will require Elma, Raspberry Pi sense hat library, Raspberry Pi and other Linux libraries

To Install needed Raspberry Pi sense hat library RTIMULib
```
sudo apt-get update
sudo apt-get install sense-hat
```
## Milestones
Port Elma to the Raspberry Pi. (Completed, "ASAN linking error on Pi" issue submitted and resolved on elma git hub)

Order and get Raspberry sense hat working (Completed, with example code)

Write code to control sense hat (Working sense hat code Project/LEDDisplay.cc)

Write code to improve elma scheduler (Added multithreading to elma) *modified client.cc to use new multithread more cleanup on client.cc possible

* https://github.com/oppiz/ECE590_Project/blob/master/include/process.h
* https://github.com/oppiz/ECE590_Project/blob/master/src/process.cc
* https://github.com/oppiz/ECE590_Project/blob/master/test/multithread.cc
* https://github.com/oppiz/ECE590_Project/blob/master/src/client.cc 

Get sense hat working in/with elma scheduler (Sense has working in elma with multithreading 
* https://github.com/oppiz/ECE590_Project/blob/master/examples/LEDDisplay.cc

## Performance

Performance can be tested/measured with ./test/bin/multithread

```
//Process decloration
SlowSingle(string name) : Process(name, 1, SINGLE) {}
SlowSingle(string name) : Process(name, 1, MULTI) {}

    //Slow sleeps a large amount of time
    void update() {
        usleep(30000);
        Slow_ADDIT++;
    }
    
    //Fast simply adds 1 and returns
    void update() {
        Fast_ADDIT++;
    }

    //populate schedualer
    m.schedule(SLEEP, 20_ms)
    .schedule(count, 3_ms)
    .init()
    .run(91_ms);

    //results
    SINGLE Fast_ADDIT: 8 Slow_ADDIT : 3
    MULTI Fast_ADDIT: 30 Slow_ADDIT : 4
```

The test file shows single threading is subject to delays under long or slow updates. Multi threading works as expected allowing for full amount of updates in the given runtime. 

### Notes 

* Because the process are ditached timing becomes odd, a sleep command in the test file was needed to ensure all update process finished before the main program exited.

