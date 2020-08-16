#include <stdlib.h>
#include <stdio.h>
#include "FpgaProtocol.h"
#include "SPILink.h"

struct FpgaProtocol {
    void (*movementCallback)(float, float, float);
    void (*colourCallback)(int, int, int);
    void (*buttonCallback)(int*);

    /*private*/ int lastX;
    /*private*/ int lastY;
    /*private*/ int lastT;
    /*private*/ SPILink* movementLink;
    /*private*/ SPILink* colourLink;
    /*private*/ SPILink* buttonLink;
};

const static float errorTolerance = 2;

const static int movementPins[] = { 0, 1, 2 };
const static int movementSize = 3;

const static int colourPins[] = { 10, 11, 12 };
const static int colourSize = 3;

const static int buttonPins[] = { 6, 7, 8 };
const static int buttonSize = 1;

const static float changeSpeed = 0.01f;
const static float thicknessModifier = 0.2f;

/*private*/ float newPos(int prev, int curr) {
    int diff = curr - prev;

    if (abs(diff) >= 255 - errorTolerance) {
        if (diff <= 0) {
            diff = curr + 255 - prev;
        } else {
            diff = curr - (prev + 255);
        }
    }

    float result = (float)diff * changeSpeed;

    result = abs(diff) > errorTolerance ? -100.0f : result;

    return result;
}

void movementParser(char* message, void* protocol) {
    FpgaProtocol* fpgaProtocol = (FpgaProtocol*) protocol;
    int _x = (int) message[0];
    int _y = (int) message[1];
    int _t = (int) message[2];

    int lastX = fpgaProtocol->lastX == -1 ? _x : fpgaProtocol->lastX;
    int lastY = fpgaProtocol->lastY == -1 ? _y : fpgaProtocol->lastY;
    int lastT = fpgaProtocol->lastT == -1 ? _t : fpgaProtocol->lastT;

    float x = newPos(lastX, _x);
    float y = newPos(lastY, _y);
    float t = newPos(lastT, _t) * thicknessModifier;

    if (x != -100.0f) {
        fpgaProtocol->lastX = _x;
    } else {
        x = 0.0f;
        fpgaProtocol->lastX = -1;
    }
    if (y != -100.0f) {
        fpgaProtocol->lastY = _y;
    } else {
        y = 0.0f;
        fpgaProtocol->lastY = -1;
    }
    if (t != -100.0f) {
        fpgaProtocol->lastT = _t;
    } else {
        t = 0.0f;
        fpgaProtocol->lastT = -1;
    }

    if (fpgaProtocol->movementCallback != NULL)
        fpgaProtocol->movementCallback(-x, y, t);
}

void colourParser(char* message, void* protocol) {
    FpgaProtocol* fpgaProtocol = (FpgaProtocol*) protocol;
    int r = (int) message[0];
    int g = (int) message[1];
    int b = (int) message[2];

    if (fpgaProtocol->colourCallback != NULL)
        fpgaProtocol->colourCallback(r, g, b);
}

void buttonParser(char* message, void* protocol) {
    FpgaProtocol* fpgaProtocol = (FpgaProtocol*) protocol;
    
    int* result = (int*) malloc(sizeof(int) * 8);
    for (int i = 0; i < 8; i++) {
        int bit = *message & (1 << (7 - i)) ? 1 : 0;
        //printf("%i ", bit);
        result[i] = bit;
    }
    //printf("\n");
    if (fpgaProtocol->buttonCallback != NULL)
        fpgaProtocol->buttonCallback(result);
}

void FpgaProtocol__init(FpgaProtocol* self) {
    self->movementCallback = NULL;
    self->colourCallback = NULL;
    self->buttonCallback = NULL;
    self->movementLink = SPILink__create(movementPins[0], movementPins[1], movementPins[2], movementSize, movementParser, self);
    self->colourLink = SPILink__create(colourPins[0], colourPins[1], colourPins[2], colourSize, colourParser, self);
    self->buttonLink = SPILink__create(buttonPins[0], buttonPins[1], buttonPins[2], buttonSize, buttonParser, self);
    self->lastX = -1;
    self->lastY = -1;
    self->lastT = -1;
}

FpgaProtocol* FpgaProtocol__create() {
    FpgaProtocol* result = (FpgaProtocol*) malloc(sizeof(FpgaProtocol));
    //printf("CREATING FPGAPROTOCOL");
    FpgaProtocol__init(result);
    return result;
}

void FpgaProtocol__setMovementCallback(FpgaProtocol* self, void (*callback)(float, float, float)) {
    self->movementCallback = callback;
}

void FpgaProtocol__setColourCallback(FpgaProtocol* self, void (*callback)(int, int, int)) {
    self->colourCallback = callback;
}

void FpgaProtocol__setButtonCallback(FpgaProtocol* self, void (*callback)(int*)) {
    self->buttonCallback = callback;
}
