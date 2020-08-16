#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiI2C.h>

int main(int argc, char** argv) {
    int fd = wiringPiI2CSetup(0x53);

    wiringPiI2CWriteReg8(fd, 0x2C, 0b00001010);
    wiringPiI2CWriteReg8(fd, 0x2D, 0b00101000);

    int prevRead = wiringPiI2CReadReg16(fd, 0x32);
    int threshold = 60000;
    int wrap = 65300;
    
    while (1) {
        int read = wiringPiI2CReadReg16(fd, 0x32);
        int diff = abs(read - prevRead);
        printf("%i\n", read);
        if (diff >= threshold && diff < wrap && read > prevRead) {
            printf("Threshold exceeded\n");
        }
        prevRead = read;
        usleep(10);
    }
}
