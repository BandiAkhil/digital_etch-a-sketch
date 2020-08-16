#ifndef SPILINK_H
#define SPILINK_H

struct SPILink;

typedef struct SPILink SPILink;

void SPILink__init(SPILink* self, int SCK, int MOSI, int SS, int maxSize, void (*callback)(char*, void*), void* userData);

SPILink* SPILink__create(int SCK, int MOSI, int SS, int maxSize, void (*callback)(char*, void*), void* userData); 

char* SPILink__getCurrentValue(SPILink* self);

#endif
