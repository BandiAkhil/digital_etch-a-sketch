#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "Line.h"
#include "UIHelpers.h"
#include "Shape.h"

struct Line {
    int r;
    int g;
    int b;
    int tranR;
    int tranG;
    int tranB;
    int transitioning;
    /*private*/ int transCount;
    /*private*/ int posChanged;
    float t;
    float maxT;
    float x;
    float maxX;
    float y;
    Shape* shape;
    float maxY;
};

void Line__init(Line* self, int r, int g, int b, float t, float x, float y, float maxT, float maxX, float maxY) {
    self->r = r;
    self->g = g;
    self->b = b;
    self->tranR = r;
    self->tranG = g;
    self->tranB = b;
    self->transCount = 0;
    self->transitioning = 0;
    self->posChanged = 0;
    self->shape = Shape__create(drawCircle, "Circle");
    self->t = t;
    self->x = x;
    self->y = y;
    self->maxT = maxT;
    self->maxX = maxX;
    self->maxY = maxY;
}

Line* Line__create(float maxT, float maxX, float maxY) {
    Line* result = (Line*) malloc(sizeof(Line));
    Line__init(result, 0, 0, 0, 0.01f, 0.0f, 0.0f, maxT, maxX, maxY);
    return result;
}

void Line__setColour(Line* self, int r, int g, int b) {
    if (!self->transitioning) {
        self->r = r;
        self->g = g;
        self->b = b;
    }
    self->tranR = r;
    self->tranG = g;
    self->tranB = b;
}

int Line__toggleColourTransition(Line* self) {
    self->transitioning = !self->transitioning;
    if (!self->transitioning) {
        self->transCount = 0;
        self->tranR = self->r;
        self->tranG = self->g;
        self->tranB = self->b;
    }
    return self->transitioning;
}

int* Line__getColour(Line* self) {
    int* values = (int*) malloc(sizeof(int) * 3);
    if (!self->transitioning) {
        values[0] = self->r;
        values[1] = self->g;
        values[2] = self->b;
    } else {
        values[0] = self->tranR;
        values[1] = self->tranG;
        values[2] = self->tranB;
    }
    return values;
}

int Line__setThickness(Line* self, float t) {
    if (t > 0.0f && t <= self->maxT) {
        self->t = t;
        return 1;
    } else {
        return 0;
    }
}

float Line__getThickness(Line* self) {
    return self->t;
}

void Line__setPosition(Line* self, float x, float y) {
    if (x >= -self->maxX && x <= self->maxX) {
        if (self->x != x)
            self->posChanged = 1;
        self->x = x;
    }
    if (y >= -self->maxY && y <= self->maxY) {
        if (self->y != y)
            self->posChanged = 1;
        self->y = y;
    }
}

float* Line__getPosition(Line* self) {
    float* values = (float*) malloc(sizeof(float) * 2);
    values[0] = self->x;
    values[1] = self->y;
    return values;
}

/*private*/ int* nextTransColour(Line* self) {
    float t = (float) self->transCount / (float) transitionSteps;
    int* result = (int*) malloc(sizeof(int) * 3);
    result[0] = (float) self->r + (float)(self->tranR - self->r) * t;
    result[1] = (float) self->g + (float)(self->tranG - self->g) * t;
    result[2] = (float) self->b + (float)(self->tranB - self->b) * t;
    return result;
}

void Line__setShape(Line* self, Shape* shape) {
    self->shape = shape;
}

void Line__draw(Line* self) {
    glBegin(GL_POLYGON);
    if (self->posChanged) {
        self->posChanged = 0;
        if (self->transitioning) {
            int* newC = nextTransColour(self);
            self->r = newC[0];
            self->g = newC[1];
            self->b = newC[2];
            //printf("tranR: %i, tranG %i, tranB: %i\n", self->tranR, self->tranG, self->tranB);
            //printf("r: %i, g: %i, b: %i\n", self->r, self->g, self->b);

            if (self->transCount == transitionSteps) {
                self->transCount = 0;
                self->transitioning = 0;
            } else {
                self->transCount++;
            }
        }
    }
    glColor3ub(self->r,self->g,self->b);
    Shape__draw(self->shape, self->x, self->y, self->t);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
}
