#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <time.h>
#include <pthread.h>

#include "motor.h"
#include "ctrlgpio.h"
#include "network.hpp"

static const int WIDTH = 320;
static const int HEIGHT = 240;
static const int YUV_FRAME_SIZE = (int)(WIDTH * HEIGHT * 1.5);
static const int FRAMERATE = 24;
static int stopFlag = 0;

static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

static void* netCam(void* arg){
    int connection = *(int*)arg;
    // Command to capture camera data
    char* videoPipeCommand;
    if (asprintf(&videoPipeCommand, "rpicam-vid --codec yuv420 -t 0 --width %d --height %d -o - -n --framerate %d", WIDTH, HEIGHT, FRAMERATE) == -1) {
        printf("Error creating videoPipeCommand\n");
        pthread_exit(NULL);
    }

    // Create a videoPipe to capture camera data
    FILE *videoPipe = popen(videoPipeCommand, "r");
    if (!videoPipe) {
        printf("Error opening videoPipe\n");
        pthread_exit(NULL);
    }

    // read and write camera stream
    while(stopFlag != 1){
        char frameBuffer[YUV_FRAME_SIZE];
        fread(frameBuffer, 1, YUV_FRAME_SIZE, videoPipe); // read from pipe
        write(connection, frameBuffer, YUV_FRAME_SIZE); // write to socket
    }

    free(videoPipeCommand);
    pthread_exit(NULL);
}

int main(){
    
    //exports the pins /sys/class/gpio/
    gpioExport(7);
    gpioExport(8);
    gpioExport(9);
    gpioExport(10);
    
    //export pwm done in motor controls

    pwmExport(0);
    pwmExport(1);
    // wait for udev
    sleepForMs(100);

    for(int i = 7; i < 11; i++){
        if (gpioExport(i) == -1) {
        perror("Error exporting GPIO pin 7");
        exit(EXIT_FAILURE);
        }
    }
    // sets the pin as either in || out 
    setDirection(7, dirOut);
    setDirection(8, dirOut);
    setDirection(9, dirOut);
    setDirection(10, dirOut);

    sleepForMs(100);

// networking stuff
    int connection = initServer();

    pthread_t camThread;
    pthread_create(&camThread, NULL, netCam, &connection);

    // loop for input and motor controls
    while (1) {

        char buff[1];
        read(connection, buff, sizeof(buff));
        if(buff[0] == 'f'){
            printf("FWD\n");
            driveForward();
        }
        if(buff[0] == 'l'){
            printf("LEFT\n");
            motorLeftFWD();
        }
        if(buff[0] == 'b'){
            printf("RVRSE\n");
            driveReverse();
        }
        if(buff[0] == 'r'){
            printf("Right\n");
            motorRightFWD();
        }
        if(buff[0] == 's'){
            stop();
        }
        if(buff[0] == 'q'){
            stop();
            stopFlag = 1;
            pthread_join(camThread, NULL);
            return 0;
        }
        
    }


    pthread_join(camThread, NULL);
    return 0;
}
