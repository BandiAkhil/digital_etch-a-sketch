#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "UIHelpers.h"
#include "Window.h"
#include "MenuBar.h"
#include "MenuList.h"
#include "WindowManager.h"

struct WindowPane {
    int selectedX;
    int selectedY;
    int selectedXMin;
    int selectedXMax;
    int selectedYMin;
    int selectedYMax;
    float orthoXMin;
    float orthoXMax;
    float orthoYMin;
    float orthoYMax;
    float width;
    float height;
    int reset;
    char* title;
    WindowManager* manager;
    void (*drawFunc)(WindowPane*, Window*);
    void (*handleActionFunc)(WindowPane*);
};

typedef struct WindowPane WindowPane;

void WindowPane__init(WindowPane* self, float width, float height, char* title) {
    self->width = width;
    self->height = height;
    self->title = title;
    self->reset = 1;
    self->manager = NULL;
    self->selectedX = 0;
    self->selectedY = -1;
    self->selectedXMin = 0;
    self->selectedXMax = 10;
    self->selectedYMin = 0;
    self->selectedYMax = 10;
    self->orthoXMin = 0.0f - width / 2;
    self->orthoXMax = 0.0f + width / 2;
    self->orthoYMin = 0.0f - height / 2;
    self->orthoYMax = 0.0f + height / 2;
    self->drawFunc = NULL;
    self->handleActionFunc = NULL;
}

void WindowPane__reset(WindowPane* self) {
    self->manager = NULL;
    self->selectedX = 0;
    self->selectedY = -1;
    self->selectedXMin = 0;
    self->selectedXMax = 10;
    self->selectedYMin = 0;
    self->selectedYMax = 10;
    self->reset = 1;
}

int WindowPane__isReset(WindowPane* self) {
    return self->reset;
}

WindowPane* WindowPane__create(float width, float height, char* title) {
    WindowPane* result = (WindowPane*) malloc(sizeof(WindowPane));
    WindowPane__init(result, width, height, title);
    return result;
}

void WindowPane__setManager(WindowPane* self, WindowManager* manager) {
    self->manager = manager;
}

WindowManager* WindowPane__getManager(WindowPane* self) {
    return self->manager;
}

void WindowPane__onUp(WindowPane* self) {
    if (self->selectedY > self->selectedYMin - 1) {
        self->reset = 0;
        self->selectedY = self->selectedY - 1;
    }
}

void WindowPane__onDown(WindowPane* self) {
    if (self->selectedY < self->selectedYMax) {
        self->reset = 0;
        self->selectedY = self->selectedY + 1;
    }
}

void WindowPane__onLeft(WindowPane* self) {
    if (self->selectedX > self->selectedXMin) {
        self->reset = 0;
        self->selectedX = self->selectedX - 1;
    }
}

void WindowPane__onRight(WindowPane* self) {
    if (self->selectedX < self->selectedXMax) {
        self->reset = 0;
        self->selectedX = self->selectedX + 1;
    }
}

void WindowPane__handleAction(WindowPane* self) {
    if (self->selectedY == self->selectedYMin - 1) {
        WindowManager__closeWindowPane(self->manager);
    }
    if (self->handleActionFunc != NULL) {
        self->handleActionFunc(self);
    }
}

float* WindowPane__getOrthoLimits(WindowPane* self) {
    float* result = (float*) malloc(sizeof(float) * 4);
    result[0] = self->orthoXMin;
    result[1] = self->orthoXMax;
    result[2] = self->orthoYMin;
    result[3] = self->orthoYMax;
    return result;
}

void WindowPane__setXLimits(WindowPane* self, int minX, int maxX) {
    self->reset = 0;
    self->selectedX = self->selectedX < minX ? minX : self->selectedX;
    self->selectedX = self->selectedX > maxX ? maxX : self->selectedX;
    self->selectedXMin = minX;
    self->selectedXMax = maxX;
}

void WindowPane__setYLimits(WindowPane* self, int minY, int maxY) {
    self->reset = 0;
    self->selectedY = self->selectedY < minY ? minY : self->selectedY;
    self->selectedY = self->selectedY > maxY ? maxY : self->selectedY;
    self->selectedYMin = minY;
    self->selectedYMax = maxY;
}

int* WindowPane__getSelected(WindowPane* self) {
    int* selected = (int*) malloc(sizeof(int) * 2);
    selected[0] = self->selectedX;
    selected[1] = self->selectedY;
    return selected;
}

void WindowPane__setSelected(WindowPane* self, int x, int y) {
    self->reset = 0;
    self->selectedX = x;
    self->selectedY = y;
}

void WindowPane__setHandleAction(WindowPane* self, void (*handleActionFunc)(WindowPane*)) {
    self->handleActionFunc = handleActionFunc;
}

void WindowPane__setDraw(WindowPane* self, void (*drawFunc)(WindowPane*, Window*)) {
    self->drawFunc = drawFunc;
}

void WindowPane__draw(WindowPane* self, Window* mainWindow) {
    float aspect = (float)Window__getWidth(mainWindow) / (float)Window__getHeight(mainWindow);
    //draw the top bar background
    glColor3ub(mainColoursDark[0], mainColoursDark[1], mainColoursDark[2]);    
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(self->orthoXMin, self->orthoYMax);
        glVertex2f(self->orthoXMin, self->orthoYMax + barHeight);
        glVertex2f(self->orthoXMax, self->orthoYMax);
        glVertex2f(self->orthoXMax, self->orthoYMax + barHeight);
    glEnd();

    //draw x button background
    if (self->selectedY == self->selectedYMin - 1) {
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
    } 
    glBegin(GL_QUADS);
        glVertex2f(self->orthoXMax - barHeight, self->orthoYMax);
        glVertex2f(self->orthoXMax - barHeight, self->orthoYMax + barHeight);
        glVertex2f(self->orthoXMax, self->orthoYMax + barHeight);
        glVertex2f(self->orthoXMax, self->orthoYMax);
    glEnd();

    //draw the title
    int titleWidth = getBitmapWidth(self->title);
    int titleHeight = getBitmapHeight();
    float relativeTitleWidth = (float)titleWidth / (float)Window__getWidth(mainWindow) * aspect;
    float relativeTitleHeight = (float)titleHeight / (float)Window__getHeight(mainWindow);
    glColor3ub(0, 0, 0);
    drawBitmapText(self->title,
        self->orthoXMin + (self->width / 2) - (float)relativeTitleWidth,
        self->orthoYMax + barHeight / 2 - (float)relativeTitleHeight / 2);

    //draw x button text
    int xWidth = getBitmapWidth("x");
    float relativeXWidth = (float)xWidth / (float)Window__getWidth(mainWindow) * aspect;
    drawBitmapText("x",
        self->orthoXMax - barHeight / 2 - (float)relativeXWidth,
        self->orthoYMax + barHeight / 2 - (float)relativeTitleHeight / 2);

    //draw the main background
    glColor3ub(mainColours[0], mainColours[1], mainColours[2]);
    glBegin(GL_QUADS);
        glVertex2f(self->orthoXMin, self->orthoYMin);
        glVertex2f(self->orthoXMin, self->orthoYMax);
        glVertex2f(self->orthoXMax, self->orthoYMax);
        glVertex2f(self->orthoXMax, self->orthoYMin);
    glEnd();

    //draw the contents
    if (self->drawFunc != NULL) {
        self->drawFunc(self, mainWindow);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
}
