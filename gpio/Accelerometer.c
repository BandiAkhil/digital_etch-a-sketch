#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <unistd.h>
#include "Accelerometer.h"

struct Accelerometer {
    void* userData;
    void (*callback)(void*);
    /*private*/ pthread_t thread;
    /*private*/ int fd;
};

typedef struct Accelerometer Accelerometer;

void *pingThread(void* userData) {
    Accelerometer* self = (Accelerometer*) userData;
    int prevRead = wiringPiI2CReadReg16(self->fd, 0x36);

    while (1) {
        int read = wiringPiI2CReadReg16(self->fd, 0x36);
        int diff = abs(read - prevRead);
        //printf("diff: %i\n", diff);
        if (diff >= threshold && diff < wrap && read > prevRead) {
            //printf("Threshold exceeded\n");
            self->callback(self->userData);
        }
        prevRead = read;
        usleep(10);
    }
}

void Accelerometer__init(Accelerometer* self, void (*callback)(void*), void* userData) {
    self->userData = userData;
    self->callback = callback;

    pinModeAlt( 8, 0b100 ); //"alt0"
    pinModeAlt( 9, 0b100 ); //"alt0"

    self->fd = wiringPiI2CSetup(0x53);

    wiringPiI2CWriteReg8(self->fd, 0x2D, 0b00101000);
    
    pthread_create(&self->thread, NULL, &pingThread, (void*) self);
}

Accelerometer* Accelerometer__create(void (*callback)(void*), void* userData) {
    Accelerometer* result = (Accelerometer*) malloc(sizeof(Accelerometer));
    Accelerometer__init(result, callback, userData);
    return result;
}
