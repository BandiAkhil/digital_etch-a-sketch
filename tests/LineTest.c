#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Line.h"


Line* testConstructor() {
    struct Line* line = Line__create();
    assert(Line__getColour(line)[0] == 0);
    assert(Line__getColour(line)[1] == 0);
    assert(Line__getColour(line)[2] == 0);
    assert(Line__getThickness(line) == 0.01f);
    assert(Line__getPosition(line)[0] == 0.00f);
    assert(Line__getPosition(line)[1] == 0.00f);
    return line;
}

void testColour(Line* line) {
    int* newColour = (int*) malloc(sizeof(int) * 3);
    newColour[0] = 128;
    newColour[1] = 129;
    newColour[2] = 130;
    Line__setColour(line, newColour[0], newColour[1], newColour[2]);
    int* getColour = Line__getColour(line);
    assert(newColour[0] == getColour[0]);
    assert(newColour[1] == getColour[1]);
    assert(newColour[2] == getColour[2]);
}

void testThickness(Line* line) {
    float newThickness = 0.02f;
    Line__setThickness(line, newThickness);
    float getThickness = Line__getThickness(line);
    assert(newThickness == getThickness);
}

void testPosition(Line* line) {
    float newX = 0.5f;
    float newY = 1.0f;
    Line__setPosition(line, newX, newY);
    float* getPosition = Line__getPosition(line);
    assert(newX == getPosition[0]);
    assert(newY == getPosition[1]);
}

int main(int argc, char** argv) {
    Line* line = testConstructor();
    testColour(line);
    testThickness(line);
    testPosition(line);
    printf("LineTest Passed\n");
}
