#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include <thread>

#include <bits/stdc++.h> 

#include "RTIMULib.h"

#define FILEPATH "/dev/fb1"
#define RGB565_RED 0x7E0
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

static vector<int> zero{0, 1, 2, 8, 10, 16, 18, 24, 26, 32, 33, 34};
static vector<int> one{1, 9, 17, 25, 33};
static vector<int> two{0, 1, 2, 10, 16, 17, 18, 24, 32, 33, 34};
static vector<int> three{0, 1, 2, 10, 16, 17, 18, 26, 32, 33, 34};
static vector<int> four{0, 2, 8, 10, 16, 17, 18, 26, 34};
static vector<int> five{0, 1, 2, 8, 16, 17, 18, 26, 32, 33, 34};
static vector<int> six{0, 1, 2, 8, 16, 17, 18, 24, 26, 32, 33, 34};
static vector<int> seven{0, 1, 2, 10, 18, 26, 34};
static vector<int> eight{0, 1, 2, 8, 10, 16, 17, 18, 24, 26, 32, 33, 34};
static vector<int> nine{0, 1, 2, 8, 10, 16, 17, 18, 26, 34};


namespace {

    using namespace elma;
    using std::vector; 

    int firstDigit(int n) { 

    while (n >= 10)  
        n /= 10; 
      
    return n; 
    } 

    int lastDigit(int n) { return (n % 10); } 

    void LEDDisplay(int n, int x, uint16_t *p){
        std::cout << "in LED Display n= " << n << " x= " << x << "\n";
        int offset = 0;
        if (x != 0){
            offset = 4;
        }
        switch (n){
            case 0: 
                for(int i = 0; i < zero.size(); i++){
                    *(p + zero[i] + offset) = RGB565_RED;
                } 
                break; 
            case 1: 
                for(int i = 0; i < one.size(); i++){
                    *(p + one[i] + offset) = RGB565_RED;
                } 
                break; 
            case 2: 
                for(int i = 0; i < two.size(); i++){
                    *(p + two[i] + offset) = RGB565_RED;
                } 
                break; 
            case 3:
                for(int i = 0; i < three.size(); i++){
                    *(p + three[i]+ offset) = RGB565_RED;
                }  
                break;
            case 4:
                for(int i = 0; i < four.size(); i++){
                    *(p + four[i] + offset) = RGB565_RED;
                }  
                break;
            case 5:
                for(int i = 0; i < five.size(); i++){
                    *(p + five[i] + offset) = RGB565_RED;
                }
                break;
            case 6:
                for(int i = 0; i < six.size(); i++){
                    *(p + six[i] + offset) = RGB565_RED;
                }
                break;
            case 7:
                for(int i = 0; i < seven.size(); i++){
                    *(p + seven[i] + offset) = RGB565_RED;
                }
                break;           
            case 8:
                for(int i = 0; i < eight.size(); i++){
                    *(p + eight[i] + offset) = RGB565_RED;
                }
                break;
            case 9:
                for(int i = 0; i < nine.size(); i++){
                    *(p + nine[i] + offset) = RGB565_RED;
                }
                break;   
            default:  
                break;   
        } 
    } 

    class  LEDScreen: public elma::Process {

     public: 
        LEDScreen(string name) : Process(name, 1, MULTI) {}
        void init() { 

            fbfd = open(FILEPATH, O_RDWR);
            if (fbfd == -1) {
                perror("Error (call to 'open')");
                exit(EXIT_FAILURE);
            }

            /* read fixed screen info for the open device */
            if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fix_info) == -1) {
                perror("Error (call to 'ioctl')");
                close(fbfd);
                exit(EXIT_FAILURE);
            }

            /* now check the correct device has been found */
            if (strcmp(fix_info.id, "RPi-Sense FB") != 0) {
                printf("%s\n", "Error: RPi-Sense FB not found");
                close(fbfd);
                exit(EXIT_FAILURE);
            }

            /* map the led frame buffer device into memory */
            //map = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
            void* fbp = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

            if (fbp == MAP_FAILED) {
                close(fbfd);
                perror("Error mmapping the file");
                exit(EXIT_FAILURE);
            }

            /* set a pointer to the start of the memory area */
            //p = map;
            map = static_cast<uint16_t*>(fbp);
            p = map;
            
        }
        void start() {}
        void update() {
            
            std::cout << "Made it to Display Update\n";
            
            if ( channel("Temperature").nonempty() ) {
                memset(map, 0, FILESIZE);
                std::cout << "Made it to Display Update Channel Loop\n";
                int TempNumber = channel("Temperature").latest();
                
                std::cout << "Temp channel is giving me: " << TempNumber << "\n";
                LEDDisplay(firstDigit(TempNumber), 0, p);
                LEDDisplay(lastDigit(TempNumber), 1, p);

            }
            //LEDDisplay(5, 0, p);
            //LEDDisplay(2, 1, p);
            
            
        }
        void stop() {
            memset(map, 0, FILESIZE);
    
            if (munmap(map, FILESIZE) == -1) {
	            perror("Error un-mmapping the file");
            }
            close(fbfd); 
        }
        private:

            int i;
            int fbfd;
            uint16_t *map;
            uint16_t *p;
            struct fb_fix_screeninfo fix_info;
            std::mutex _mtx;

    };    

    class  Temp: public elma::Process {

    public: 
        Temp(string name) : Process(name) {}
        void init() {
            settings = new RTIMUSettings("RTIMULib");
            imu = RTIMU::createIMU(settings);
            humidity = RTHumidity::createHumidity(settings);

            if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
                printf("No IMU found\n");
                exit(1);
            }

            imu->IMUInit();

            if (humidity != NULL)
                humidity->humidityInit();

        }
        void start() {}
        void update() {

            usleep(imu->IMUGetPollInterval() * 1000);

            while (imu->IMURead()) {
                RTIMU_DATA imuData = imu->getIMUData();

                if (humidity != NULL)
                    humidity->humidityRead(imuData);
    
                printf("temperature: %4.1f \n", imuData.temperature *(9/5)+32);
                int Ttemp = (int) (imuData.temperature*(9/5)+32);
                channel("Temperature").send(Ttemp);
            }
 
        }
        void stop() {}
        private:

        RTIMUSettings *settings; 
        RTIMU *imu;
        RTHumidity *humidity; 

    };    


}

    int main(void){              
    
        elma::Manager m;
        LEDScreen display("display");
        Temp ReadTemp("ReadTemp");
        Channel Temperature("Temperature");

        m.schedule(display, 3000_ms)
         .schedule(ReadTemp, 3000_ms)
         .add_channel(Temperature)
         .init()
         .run(60000_ms); 
        
    }