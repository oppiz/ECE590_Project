#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include <bits/stdc++.h> 

#include "RTIMULib.h"

#include <vector>

using namespace std;

#define FILEPATH "/dev/fb1"
#define RGB565_RED 0x7E0
#define RGB565_BLACK 0x0000
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

static vector<int> one{1, 9, 17, 25, 33};
static vector<int> two{0, 1, 2, 10, 16, 17, 18, 24, 32, 33, 34};
static vector<int> three{0, 1, 2, 10, 16, 17, 18, 26, 32, 33, 34};
static vector<int> four{0, 2, 8, 10, 16, 17, 18, 26, 34};
static vector<int> five{0, 1, 2, 8, 16, 17, 18, 26, 32, 33, 34};
static vector<int> six{0, 1, 2, 8, 16, 17, 18, 24, 26, 32, 33, 34};
static vector<int> seven{0, 1, 2, 10, 18, 26, 34};
static vector<int> eight{0, 1, 2, 8, 10, 16, 17, 18, 24, 26, 32, 33, 34};
static vector<int> nine{0, 1, 2, 8, 10, 16, 17, 18, 26, 34};

int firstDigit(int n) 
{ 
    // Remove last digit from number 
    // till only one digit is left 
    while (n >= 10)  
        n /= 10; 
      
    // return the first digit 
    return n; 
} 

int lastDigit(int n) 
{ 
    // return the last digit 
    return (n % 10); 
} 

void LEDDisplay(int n, int x, uint16_t *p){
    int offset = 0;
    if (x != 0){
        offset = 4;
    }
    switch (n){
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

int main(int argc, char **argv) {
    
    int i;
    int fbfd;
    uint16_t *map;
    uint16_t *p;
    struct fb_fix_screeninfo fix_info;



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

    //Temp stuff is read from the humidity sensor
    RTIMUSettings *settings = new RTIMUSettings("RTIMULib");
    RTIMU *imu = RTIMU::createIMU(settings);
    RTHumidity *humidity = RTHumidity::createHumidity(settings);

    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }

    imu->IMUInit();

    if (humidity != NULL)
        humidity->humidityInit();


    /* clear the led matrix */
    memset(map, 0, FILESIZE);

    //*p = RGB565_RED;
    //*(p+12) = RGB565_RED;
    //*(p+1) = RGB565_RED;

    while (1) {
        //  poll at the rate recommended by the IMU

        usleep(imu->IMUGetPollInterval() * 1000);

        while (imu->IMURead()) {
            RTIMU_DATA imuData = imu->getIMUData();


            if (humidity != NULL)
                humidity->humidityRead(imuData);
   
            printf("temperature: %4.1f \n", imuData.temperature *(9/5)+32);
            int temp = (int) (imuData.temperature*(9/5)+32);
            memset(map, 0, FILESIZE);
            LEDDisplay(firstDigit(temp), 0, p);
            LEDDisplay(lastDigit(temp), 1, p);

            sleep(1);
        }
    }

    //for(int j = 0; j < 10000000; j++){

    //    for(int i = 0; i < nine.size(); i++){
    //        *(p + nine[i]) = RGB565_RED;
            //usleep(2);
    //        *(p + nine[i]) = RGB565_BLACK;
    //    }
    //}    

    //clear leds
    //sleep(10);
    memset(map, 0, FILESIZE);
    
    if (munmap(map, FILESIZE) == -1) {
	    perror("Error un-mmapping the file");
    }
    close(fbfd);    

    return 0;

}