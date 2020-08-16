#ifndef WINDOWPANE_H
#define WINDOWPANE_H
#include "Window.h"

struct WindowPane;

struct WindowManager;

typedef struct WindowManager WindowManager;

typedef struct WindowPane WindowPane;

void WindowPane__init(WindowPane* self, float width, float height, char* title);

void WindowPane__reset(WindowPane* self);

int WindowPane__isReset(WindowPane* self);

WindowPane* WindowPane__create(float width, float height, char* title);

void WindowPane__setManager(WindowPane* self, WindowManager* manager);

WindowManager* WindowPane__getManager(WindowPane* self);

void WindowPane__onUp(WindowPane* self);

void WindowPane__onDown(WindowPane* self);

void WindowPane__onLeft(WindowPane* self);

void WindowPane__onRight(WindowPane* self);

void WindowPane__handleAction(WindowPane* self);

float* WindowPane__getOrthoLimits(WindowPane* self);

void WindowPane__setXLimits(WindowPane* self, int minX, int maxX);

void WindowPane__setYLimits(WindowPane* self, int minY, int maxY);

int* WindowPane__getSelected(WindowPane* self);

void WindowPane__setSelected(WindowPane* self, int x, int y);

void WindowPane__setHandleAction(WindowPane* self, void (*handleActionFunc)(WindowPane*));

void WindowPane__draw(WindowPane* self, Window* mainWindow);

void WindowPane__setDraw(WindowPane* self, void (*drawFunc)(WindowPane*, Window*));

#endif
