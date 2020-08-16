#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

struct Accelerometer;

typedef struct Accelerometer Accelerometer;

const static int wrap = 65500;
const static int threshold = 60000;

void Accelerometer__init(Accelerometer* self, void (*callback)(void*), void* userData);

Accelerometer* Accelerometer__create(void (*callback)(void*), void* userData);

#endif
