#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include "Overlay.h"
#include "UIHelpers.h"
#include "Window.h"
#include "Style.h"

struct Overlay {
    int r;
    int g;
    int b;
    float t;
    char** messages;
    /*private*/ int currentCycle;
    /*private*/ int messageCount;
};

typedef struct Overlay Overlay;

void Overlay__init(Overlay* self) {
    self->r = 0;
    self->g = 0;
    self->b = 0;
    self->t = 0.01f;
    self->messages = (char**) malloc(sizeof(char*) * messageMax);
    self->currentCycle = 0;
    self->messageCount = 0;
}

Overlay* Overlay__create() {
    Overlay* result = (Overlay*) malloc(sizeof(Overlay));
    Overlay__init(result);
    return result;
}

void Overlay__setColour(Overlay* self, int r, int g, int b) {
    self->r = r;
    self->g = g;
    self->b = b;
}

void Overlay__setThickness(Overlay* self, float t) {
    self->t = t;
}

void Overlay__addMessage(Overlay* self, char* message) {
    for (int i = self->messageCount; i >= 0; i--) {
        self->messages[i + 1] = self->messages[i];
    }
    if (self->messageCount < messageMax) {
        self->messageCount++;
    }
    self->messages[0] = message;
}

void Overlay__removeMessage(Overlay* self) {
    if (self->messageCount > 0) {
        self->messages[self->messageCount - 1] = '\0';
        self->messageCount--;
    }
}

void Overlay__draw(Overlay* self, Window* mainWindow) {
    float aspect = (float)Window__getWidth(mainWindow) / (float)Window__getHeight(mainWindow);

    int spaceWidth = getBitmapWidth(" ");
    float relativeSpaceWidth = (float)spaceWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

    int textHeight = getBitmapHeight();
    float relativeTextHeight = (float)textHeight / (float)Window__getHeight(mainWindow);

    //draw red
    char rstring[10];
    snprintf(rstring, 10, "R: %i", self->r);
    int rWidth = getBitmapWidth(rstring);
    float relativeRWidth = (float)rWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

    glColor3ub(self->r, 0, 0);
    drawBitmapText(rstring, -aspect + relativeSpaceWidth, -1.0f + relativeSpaceWidth);

    //draw green
    char gstring[10];
    snprintf(gstring, 10, "G: %i", self->g);
    int gWidth = getBitmapWidth(gstring);
    float relativeGWidth = (float)gWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

    glColor3ub(0, self->g, 0);
    drawBitmapText(gstring, -aspect + relativeRWidth + relativeSpaceWidth * 2, -1.0f + relativeSpaceWidth);

    //draw blue
    char bstring[10];
    snprintf(bstring, 10, "B: %i", self->b);
    int bWidth = getBitmapWidth(bstring);
    float relativeBWidth = (float)bWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

    glColor3ub(0, 0, self->b);
    drawBitmapText(bstring, -aspect + relativeRWidth + relativeGWidth + relativeSpaceWidth * 3, -1.0f + relativeSpaceWidth);

    //draw colour square
    glColor3ub(self->r, self->g, self->b);
    glBegin(GL_QUADS);
        glVertex2f(-aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeSpaceWidth * 4, -1.0f + relativeSpaceWidth);
        glVertex2f(-aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeSpaceWidth * 4, -1.0f + relativeSpaceWidth * 2);
        glVertex2f(-aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeSpaceWidth * 5, -1.0f + relativeSpaceWidth * 2);
        glVertex2f(-aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeSpaceWidth * 5, -1.0f + relativeSpaceWidth);
    glEnd();

    //draw thickness
    char* tstring = "T:";
    int tWidth = getBitmapWidth(tstring);
    float relativeTWidth = (float)tWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

    glColor3ub(0, 0, 0);
    drawBitmapText(tstring, -aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeSpaceWidth * 6, -1.0f + relativeSpaceWidth);

    //draw circle
    glColor3ub(self->r, self->g, self->b);
    drawCircle(-aspect + relativeRWidth + relativeGWidth + relativeBWidth + relativeTWidth + relativeSpaceWidth * 7 + self->t, -1.0f + relativeSpaceWidth + self->t, self->t);

    //draw messages
    for (int i = 0; i < self->messageCount; i++) {
        char* message = self->messages[i];
        int messageWidth = getBitmapWidth(message);
        float relativeMessageWidth = (float)messageWidth / (float)Window__getWidth(mainWindow) * aspect * 2;

        glColor3ub(0, 0, 0);
        drawBitmapText(message, aspect - relativeMessageWidth - relativeSpaceWidth, 1.0f - barHeight - (relativeTextHeight + relativeSpaceWidth) * (i + 1));
    }

    self->currentCycle++;

    if (self->currentCycle == messageAliveCycles) {
        Overlay__removeMessage(self);
        self->currentCycle = 0;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
}
