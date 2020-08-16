#include "Mapping.h"

const static int pinMapping[] = { 16, 15, 7, 9, 8, 6, 5, 4, 3, 2, 1, 0, 11, 10, 14, 13, 12, 23, 26, /*don't use*/ -1, 21, 31, 30, 29, 28, 25, 27, 24 };

int GPIO(int fpgaPin) {
    return pinMapping[fpgaPin];
}
