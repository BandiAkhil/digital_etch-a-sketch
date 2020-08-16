#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "SPILink.h"
#include "Mapping.h"

struct SPILink {
    int SCK;
    int MOSI;
    int SS;
    void (*callback)(char*, void*);
    char* currentValue;
    int messageSize;

    /*private*/ char* buffer;
    /*private*/ int bufferClear;
    /*private*/ int messageI;
    /*private*/ int maxSize;
    /*private*/ int SSActive;
    /*private*/ void* userData;
    /*private*/ int SSInit;
};

typedef struct SPILink SPILink;

/*private*/ void shiftl(char* buff, int size) {
    for (int i = 0; i < size; i++) {
        int bit = 0;
        if (i < size - 1) {
            bit = (buff[i + 1] & 128) >> 7;
        }
        buff[i] = buff[i] << 1;
        buff[i] |= bit;
    }
}

void SCKInterrupt(void* link) {
    SPILink* spiLink = (SPILink*) link;
    if (!spiLink->SSActive) {
        int val = digitalRead(spiLink->MOSI);
        //printf("%i", val);
        spiLink->bufferClear = 0;
        int byteI = spiLink->messageI % 8;
        int charI = spiLink->messageI / 8;
        if (val)
            spiLink->buffer[charI] |= 1UL << 7 - byteI;
        spiLink->messageI++;
    } 
}

void *callBack(void* input) {
    SPILink* link = (SPILink*) input;
}

void SSInterrupt(void* link) {
    //printf("SS INTERRUPTED\n");
    SPILink* spiLink = (SPILink*) link;
    spiLink->SSActive = digitalRead(spiLink->SS);
    if (spiLink->SSActive && !spiLink->bufferClear) {
        int printOutput = 0;

        if (printOutput) {
            printf("printing message: ");
            for (int i = 0; i < spiLink->maxSize; i++) {
                for (int j = 0; j < 8; j++) {
                    printf("%d", !!((spiLink->buffer[i] << j) & 0x80));
                }
                printf(" ");
            }
            printf("\n");
        }

        
        //strcpy(spiLink->currentValue, spiLink->buffer);
        pthread_t t;
        pthread_create(&t, NULL, callBack, link);
        spiLink->callback(spiLink->buffer, spiLink->userData);
        //printf("made it here 0\n");
        
        memset(spiLink->buffer, 0, sizeof(char) * spiLink->maxSize);
        spiLink->messageI = 0;
        spiLink->bufferClear = 1;
    }
}

void SPILink__init(SPILink* self, int SCK, int MOSI, int SS, int maxSize, void (*callback)(char*, void*), void* userData) {
    self->SCK = GPIO(SCK);
    self->MOSI = GPIO(MOSI);
    self->SS = GPIO(SS);
    self->callback = callback;
    self->maxSize = maxSize;
    self->bufferClear = 1;
    self->messageI = 0;
    self->userData = userData;
    self->buffer = (char*) malloc(sizeof(char) * maxSize);
    memset(self->buffer, 0, sizeof(char) * self->maxSize);
    self->currentValue = (char*) malloc(sizeof(char) * maxSize);
    self->messageSize = 0;

    pinMode(self->SCK, INPUT);
    pinMode(self->MOSI, INPUT);
    pinMode(self->SS, INPUT);    

    //printf("SETTING INTERRUPTS FOR SCK=%i, MOSI=%i, SS=%i\n", SCK, MOSI, SS);
    wiringPiISR(self->SCK, INT_EDGE_RISING, SCKInterrupt, (void*) self);
    self->SSActive = digitalRead(self->SS);
    self->SSInit = 0;
    wiringPiISR(self->SS, INT_EDGE_BOTH, SSInterrupt, (void*) self);
}

SPILink* SPILink__create(int SCK, int MOSI, int SS, int maxSize, void (*callback)(char*, void*), void* userData) {
    //printf("CREATING SPILINK\n");
    SPILink* result = (SPILink*) malloc(sizeof(SPILink));
    SPILink__init(result, SCK, MOSI, SS, maxSize, callback, userData);
    return result;
}

char* SPILink__getCurrentValue(SPILink* self) {
    return self->currentValue;
}
