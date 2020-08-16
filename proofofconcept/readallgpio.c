#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void cleanUp(int sig) {
    //gpioTerminate();
    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, cleanUp);
    wiringPiSetup();
    for (int i = 0; i <= 26; i++) {
        pinMode(i, INPUT);
    }
    while (1) {
        for (int i = 0; i <= 27; i++) {
            int val = digitalRead(i);
            printf("Pin %i = %i\n", i, val);
            if (i == 4 && val == 1 && 0) {
                printf("BIG LETTERS TO INDICATE THAT 4 IS 1 RIGHT NOW AND NOT A FLICKER\n");
                printf("BIG LETTERS TO INDICATE THAT 4 IS 1 RIGHT NOW AND NOT A FLICKER\n");
                printf("BIG LETTERS TO INDICATE THAT 4 IS 1 RIGHT NOW AND NOT A FLICKER\n");
                printf("BIG LETTERS TO INDICATE THAT 4 IS 1 RIGHT NOW AND NOT A FLICKER\n");
                printf("BIG LETTERS TO INDICATE THAT 4 IS 1 RIGHT NOW AND NOT A FLICKER\n");
            }
        }
        printf("\n");
        usleep(10000);
    }
}
