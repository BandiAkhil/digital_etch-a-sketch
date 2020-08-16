#ifndef LINE_H
#define LINE_H
#include "Shape.h"

struct Line;

typedef struct Line Line;

const static int transitionSteps = 2500;

void Line__init(Line* self, int r, int g, int b, float t, float x, float y, float maxT, float maxX, float maxY);

Line* Line__create(float maxT, float maxX, float maxY);

void Line__setColour(Line* self, int r, int g, int b);

int Line__toggleColourTransition(Line* self);

int* Line__getColour(Line* self);

int Line__setThickness(Line* self, float t);

float Line__getThickness(Line* self);

void Line__setPosition(Line* self, float x, float y);

float* Line__getPosition(Line* self);

void Line__setShape(Line* self, Shape* shape);

void Line__draw(Line* self);

#endif 
