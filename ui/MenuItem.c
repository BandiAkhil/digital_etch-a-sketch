#include <GL/glut.h>
#include <stdio.h>
#include "MenuBar.h"
#include "MenuItem.h"
#include "MenuList.h"
#include "Window.h"
#include "UIHelpers.h"

struct MenuItem {
    char* title;
    void (*onClick)(void*);
    void* userData;
};

typedef struct MenuItem MenuItem;

void MenuItem__init(MenuItem* self, char* title, void (*onClick)(void*), void* userData) {
    self->title = title;
    self->onClick = onClick;
    self->userData = userData;
}

MenuItem* MenuItem__create(char* title, void (*onClick)(void*), void* userData) {
    MenuItem* result = (MenuItem*) malloc(sizeof(MenuItem));
    MenuItem__init(result, title, onClick, userData);
    return result;
}

void MenuItem__setOnClick(MenuItem* self, void (*onClick)(void*)) {
    self->onClick = onClick;
}

void MenuItem__callOnClick(MenuItem* self) {
    if (self->onClick != NULL) {
        self->onClick(self->userData);
    }
}

void MenuItem__setTitle(MenuItem* self, char* title) {
    self->title = title;
}

void MenuItem__draw(MenuItem* self, int listIndex, int index, int selected, Window* mainWindow) {
    float aspect = (float)Window__getWidth(mainWindow) / (float)Window__getHeight(mainWindow);

    float x0 = listWidth * listIndex;
    float x1 = listWidth * (listIndex + 1);

    //draw background
    if (selected) {
        glColor3ub(mainColoursLight[0], mainColoursLight[1], mainColoursLight[2]);
    } else {
        glColor3ub(mainColours[0], mainColours[1], mainColours[2]);
    }
    glBegin(GL_QUADS);
        glVertex2f(-aspect + x0, 1.0f - barHeight * (index + 2));
        glVertex2f(-aspect + x0, 1.0f - barHeight * (index + 1));
        glVertex2f(-aspect + x1, 1.0f - barHeight * (index + 1));
        glVertex2f(-aspect + x1, 1.0f - barHeight * (index + 2));
    glEnd();

    //draw title
    int titleWidth = getBitmapWidth(self->title);
    int titleHeight = getBitmapHeight();
    float relativeTitleWidth = (float)titleWidth / (float)Window__getWidth(mainWindow) * aspect;
    float relativeTitleHeight = (float)titleHeight / (float)Window__getHeight(mainWindow);
    
    glColor3ub(0, 0, 0);
    drawBitmapText(self->title,
        (-aspect + x0) + (listWidth / 2) - (float)relativeTitleWidth,
        1.0f - barHeight * (index + 2) + barHeight / 2 - (float)relativeTitleHeight / 2);

    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2f(-aspect + x1, 1.0f - barHeight * (index + 2));
        glVertex2f(-aspect + x1, 1.0f - barHeight * (index + 1));
    glEnd();
    glColor3ub(255, 255, 255);
}
