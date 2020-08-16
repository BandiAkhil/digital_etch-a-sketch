#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
//#include "../lib/fpgacom.h"

const static int movSCK = 16;
const static int movMOSI = 15;
const static int movSS = 7;

pthread_cond_t movCond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t movLock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t movInpCond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t movInpLock = PTHREAD_MUTEX_INITIALIZER;

int sshiint = 0;
int waitingSS = 0;

PI_THREAD (readMovThread) {
    piHiPri(10);
    //wait for SS
    char buff[3];
    int bufclr = 1;
    int arri = 0;
    int bytei = 0;
    while (1) {
        if (digitalRead(movSS) == 1) {
            if (!bufclr) {
                printf("\nprinting message: ");
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 8; j++) {
                        printf("%d", !!((buff[i] << j) & 0x80));
                    }
                    printf(" ");
                }
                printf("\n");
                memset(buff, 0, sizeof buff);
                arri = 0;
                bytei = 0;
                bufclr = 1;
                //break;
            }
            pthread_mutex_lock(&movLock);
            printf("WAITING\n");
            waitingSS = 1;
            pthread_cond_wait(&movCond, &movLock);
            waitingSS = 0;
            printf("INTERRUPTED\n");
            pthread_mutex_unlock(&movLock);
        }
        pthread_mutex_lock(&movInpLock);
        pthread_cond_wait(&movInpCond, &movInpLock);
        pthread_mutex_unlock(&movInpLock);
        if (sshiint) {
            sshiint = 0;
            continue;
        }
        int val = digitalRead(movMOSI);
        printf("%i", val);
        bufclr = 0;
        buff[arri] += val;
        bytei++;
        if (bytei == 8) {
            arri++;
            bytei = 0;
        } else {
            buff[arri] = buff[arri] << 1;
        }
    } 
}

void interruptMovThread(void) {
    //printf("INNTERRUPTDSFM\n");
    if (waitingSS) {
        pthread_cond_signal(&movCond);
    } else {
        sshiint = 1;
        pthread_cond_signal(&movInpCond);
    }
}

void interruptMovInput(void) {
    pthread_cond_signal(&movInpCond);
}

struct timespec ns = {0, 10000};

void sendCharToMov(unsigned char data) {
    for (int i = 0; i < 8; i++) {
        int toSend = (data >> (7 - i)) & 1;
        digitalWrite(27,0);
        digitalWrite(22, toSend);
        nanosleep(&ns, NULL);
        digitalWrite(27, 1);
        nanosleep(&ns, NULL);
    }
}

void cleanUp(int sig) {
    //gpioTerminate();
    exit(0);
}

int main(int argc, char** argv) {
    int simul = 0;
    if (argc > 1 && strcmp(argv[1], "-s"))
        simul = 1;
    //printf("%i", simul);
    signal(SIGINT, cleanUp);

    //initialize GPIO
    if (wiringPiSetup() < 0) {
        printf("GPIO failed to initialize");
        return 1;
    }
//    gpioSetMode(18, PI_INPUT);
//    gpioSetMode(17, PI_OUTPUT);
//    gpioSetMode(27, PI_OUTPUT);
//    gpioSetMode(22, PI_OUTPUT);
    pinMode(movSCK, INPUT);
    pinMode(movMOSI, INPUT);
    pinMode(movSS, INPUT);

    //set SS to high
//    gpioWrite(17, 1);
    //set SCK to high
//    gpioWrite(27, 1);
    //set MOSI to low
//    gpioWrite(22, 1);

    //start thread to read GPIO1, 2, and 3
    wiringPiISR(movSS, INT_EDGE_BOTH, &interruptMovThread);
    wiringPiISR(movSCK, INT_EDGE_RISING, &interruptMovInput);
    piThreadCreate(readMovThread);
    //define message to be sent
    unsigned char x = 0b10000010;
    unsigned char y = 0b00001000;
    unsigned char th = 0b00010000;

    //wait for button click to start sending data
    while (1) {
/*
        if (gpioRead(18) == 1) {
            //send mov data
            gpioWrite(17, 0);
            nanosleep(&ns, NULL);
            nanosleep(&ns, NULL);
            sendCharToMov(x);
            sendCharToMov(y);
            sendCharToMov(th);
            gpioWrite(17, 1);
            sleep(1);
        }
*/
    }
}
