#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>
#include "FpgaProtocol.h"
#include "Accelerometer.h"

void handleMovement(float x, float y, float t) {
    printf("Received movement with x=%f, y=%f, and t=%f\n", x, y, t);
}

void handleColour(int r, int g, int b) {
    printf("Received colour with r=%i, g=%i, and b=%i\n", r, g, b);
}

void handleButton(int* inputs) {
    //printf("Still haven't implemented this shit");
}

void handleReset(void* data) {
    printf("Reset activated!\n");
}

int main(int argc, char** argv) {
    wiringPiSetup();

    FpgaProtocol* protocol = FpgaProtocol__create();
    FpgaProtocol__setMovementCallback(protocol, handleMovement);
    FpgaProtocol__setColourCallback(protocol, handleColour);
    FpgaProtocol__setButtonCallback(protocol, handleButton);
    //Accelerometer* accelerometer = Accelerometer__create(handleReset, NULL);
    while (1) {
        sleep(1);
        fflush(stdout);
    }
}
